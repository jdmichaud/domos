#include <iostream>
#include <mutex>
#include <condition_variable>
#include "protocol.h"
#include "verbose.h"
#include "RF33_adapter.hpp"

// RFSwitch library only accepts C function pointer.
static std::mutex m;
static std::condition_variable cv;

void transmissionHandler() {
  // A signal is being received, wake up program
  cv.notify_all();
}

int RF33Adapter::init(int pin) {
  // Init wiring PI
  if(wiringPiSetupSys() == -1) {
    std::cerr << "wiringPiSetup failed, exiting..." << std::endl;
    return 1;
  }
  // Init RF Switch
  // if (pulseLength != 0) _rfSwitch.setPulseLength(pulseLength);
  _rfSwitch.enableReceive(pin);
  _rfSwitch.registerCustomInterruptHandler(transmissionHandler);
  return 0;
}

int RF33Adapter::receiveMessage(std::function<void(const packet_s &)> callback) {
  bool receiving_first = true;
  packet_t value = 0;
  packet_t previous_value = 0;
  uint64_t count = 0;
  VERBOSE(std::cout << "listening..." << std::endl);
  while(1) {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);

    if (_rfSwitch.available()) {
      value = _rfSwitch.getReceivedValue();
      // std::cout << value << std::endl;
      _rfSwitch.resetAvailable();
      count++;
      VERBOSE(std::cout << "value received (" << count << "): " << value << std::endl);
      if (value == previous_value)
        continue; // If we receive the same value, this is just a repeat
      previous_value = value;
      // Now unpack it
      packet_s packet;
      int ret = read_packet(value, &packet);
      if (ret == WRONG_MAGIC_ERROR) {
        std::cout << "DEBUG: Wrong magic number, dismiss message" << std::endl;
        continue;
      } else if (ret == PARITY_ERROR) {
        std::cerr << "ERROR: Parity error in message, tread lightly" << std::endl;
      }
      callback(packet);
      value = 0;
    }
  }

}

