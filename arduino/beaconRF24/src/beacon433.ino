#include <RF24.h>

#define PING_MESSAGE "ping!"

int chip_enable_pin = 2;
int chip_select_pin = 3;

RF24 rfDevice(chip_enable_pin, chip_select_pin);

byte addresses[][6] = { "GeoG1", "GeoG2" };

void setup() {
  Serial.begin(115200);
  // Setup and configure rf radio
  rfDevice.begin(); // Start up the radio
  rfDevice.setAutoAck(1); // Ensure autoACK is enabled
  rfDevice.setRetries(15,15); // Max delay between retries & number of retries
  rfDevice.openWritingPipe(addresses[1]); // Write to device address 'GeoG2'
  // rfDevice.openReadingPipe(1,addresses[0]); // Read on pipe 1 for device address 'GeoG1'
  // rfDevice.startListening(); // Start listening
}

void loop() {
  int timestamp = millis();

  if (!rfDevice.write(PING_MESSAGE, strlen(PING_MESSAGE))) {
    Serial.println(F("failed to transmit"));
  } else {
    Serial.println(F("ping sent"));
  }

  // Send a ping every second
  delay(1000 - (timestamp - millis()));
}