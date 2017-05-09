#include <RF24.h>

int chip_enable_pin = 2;
int chip_select_pin = 3;

RF24 rfDevice(chip_enable_pin, chip_select_pin);

byte addresses[][6] = { "1Node", "2Node" };

void setup() {
  // Setup and configure rf radio
  radio.begin(); // Start up the radio
  radio.setAutoAck(1); // Ensure autoACK is enabled
  radio.setRetries(15,15); // Max delay between retries & number of retries
  radio.openWritingPipe(addresses[1]); // Write to device address '2Node'
  radio.openReadingPipe(1,addresses[0]); // Read on pipe 1 for device address '1Node'
  radio.startListening(); // Start listening
}

void loop() {
}