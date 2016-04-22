#include <mutex>
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
  std::cout << "receiving..." << std::endl;
  cv.notify_all();
}

int main(int argc, char **argv) {
  RCSwitch rfSwitch;

  if(wiringPiSetup() == -1) {
    std::cout << "wiringPiSetup failed, exiting..." << std::endl;
    return 0;
  }

  int pulseLength = 0;
  if (argv[1] != NULL) pulseLength = atoi(argv[1]);
  if (pulseLength != 0) rfSwitch.setPulseLength(pulseLength);
  rfSwitch.enableReceive(RF_DEVICE_COMM_PIN);  // Receiver on interrupt 0 => that is pin #2
  rfSwitch.registerCustomInterruptHandler(transmissionHandler);

  while(1) {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);

    if (rfSwitch.available()) {
      int value = rfSwitch.getReceivedValue();
      if (value == 0) {
        std::cout << "Unknown encoding" << std::endl;
      } else {
        std::cout << "Received " << value << std::endl;
      }
      rfSwitch.resetAvailable();
    }
  }

  return 0;
}