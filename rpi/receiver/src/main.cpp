#include <mutex>
#include <stdint.h>
#include <iostream>
#include <condition_variable>
#include "protocol.h"
#include "RCSwitch.h"

// The RF device data pin
#define RF_DEVICE_COMM_PIN 2

std::mutex m;
std::condition_variable cv;

void transmissionHandler() {
  // A signal is being received, wake up program
  cv.notify_all();
}

int init(RCSwitch& rfSwitch, int pulseLength) {
  // Init wiring PI
  if(wiringPiSetup() == -1) {
    std::cerr << "wiringPiSetup failed, exiting..." << std::endl;
    return 1;
  }
  // Init RF Switch
  if (pulseLength != 0) rfSwitch.setPulseLength(pulseLength);
  rfSwitch.enableReceive(RF_DEVICE_COMM_PIN);  // Receiver on interrupt 0 => that is pin #2
  std::cout << "listening..." << std::endl;
  rfSwitch.registerCustomInterruptHandler(transmissionHandler);
}

void processPacket(packet_s packet) {
  std::cout << "sensor type: " << (int) packet.stype <<
             ", sensor id: " << (int) packet.sid <<
             ", message: " << (int) packet.message << std::endl;

}

void receiveMessage(RCSwitch& rfSwitch) {
  bool receiving_first = true;
  uint64_t previous_value = 0;
  uint64_t value = 0;
  packet_t raw_packet = 0;
  while(1) {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);

    if (rfSwitch.available()) {
      value = rfSwitch.getReceivedValue();
      rfSwitch.resetAvailable();
      if (value == previous_value)
        continue; // If we receive the same value, this is just a repeat
      previous_value = value;
      raw_packet |= value;
      // If we are receiving the first part of the packet, shift the value by
      // 32 to let room for the second half
      if (receiving_first) {
        raw_packet <<= 32;
        receiving_first = false;
      } else {
        // We received the second part, process it
        packet_s packet;
        int ret = read_packet(raw_packet, &packet);
        if (ret == WRONG_MAGIC_ERROR) {
          std::cout << "DEBUG: Wrong magic number, dismiss message" << std::endl;
          continue;
        } else if (ret == PARITY_ERROR) {
          std::cerr << "ERROR: Parity error in message, tread lightly" << std::endl;
        }
        processPacket(packet);
        receiving_first = true;
        raw_packet = 0;
        value = 0;
      }
    }
  }
}

int main(int argc, char **argv) {
  RCSwitch rfSwitch;

  if (argv[1] != NULL) {
    int retcode = 0;
    if ((retcode = init(rfSwitch, atoi(argv[1]))) != 0);
      return retcode;
  }

  receiveMessage(rfSwitch);

  return 0;
}


