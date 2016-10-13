#include <stdint.h>
#include <iostream>
#include "protocol.h"
#include "RF33_adapter.hpp"

// The RF device data pin
#define RF_DEVICE_COMM_PIN 27

void processPacket(const packet_s &packet) {
  std::cout << "sensor type: " << (int) packet.stype <<
             ", sensor id: " << (int) packet.sid <<
             ", message: " << (int) packet.message << std::endl;

}

int main(int argc, char **argv) {
  RF33Adapter rf33;

  int retcode;
  if ((retcode = rf33.init(RF_DEVICE_COMM_PIN)) != 0) {
    std::cout << "RF33 initialization failed with " << retcode << std::endl;
    return retcode;
  }
  rf33.receiveMessage(processPacket);

  return 0;
}



