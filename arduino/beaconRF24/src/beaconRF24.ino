#include <RF24.h>

#define PING_MESSAGE "ping!"

int SERIAL_COMMUNICATION_SPEED = 9600;
// Pin on which the RF device is plugged;
int RF_ENABLE_PIN = 2;
int RF_SELECT_PIN = 3;
// Pin on which the RF device is plugged;
int LED_PIN = 9;

RF24 rfDevice(RF_ENABLE_PIN, RF_SELECT_PIN);

byte addresses[][6] = { "GeoG1", "GeoG2" };

void setup() {
  Serial.begin(SERIAL_COMMUNICATION_SPEED);
  // Setup and configure rf radio
  rfDevice.begin(); // Start up the radio
  rfDevice.setAutoAck(1); // Ensure autoACK is enabled
  rfDevice.setRetries(15,15); // Max delay between retries & number of retries
  rfDevice.openWritingPipe(addresses[1]); // Write to device address 'GeoG2'
  // rfDevice.openReadingPipe(1,addresses[0]); // Read on pipe 1 for device address 'GeoG1'
  // rfDevice.startListening(); // Start listening

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int timestamp = millis();

  digitalWrite(LED_PIN, 1);
  if (!rfDevice.write(PING_MESSAGE, strlen(PING_MESSAGE))) {
    Serial.println(F("failed to transmit"));
  } else {
    Serial.println(F("ping sent"));
  }
  digitalWrite(LED_PIN, 0);

  // Send a ping every second
  delay(1000 - (timestamp - millis()));
}