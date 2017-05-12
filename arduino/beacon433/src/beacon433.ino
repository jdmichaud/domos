#include <RCSwitch.h>

#define PING_MESSAGE "01010101"

int SERIAL_COMMUNICATION_SPEED = 9600;
// Pin on which the RF device is plugged;
int RF_DEVICE_COMM_PIN = 13;
// Pin on which the RF device is plugged;
int LED_PIN = 9;

RCSwitch rfDevice = RCSwitch();

void setup() {
  Serial.begin(SERIAL_COMMUNICATION_SPEED);
  pinMode(RF_DEVICE_COMM_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  rfDevice.enableTransmit(RF_DEVICE_COMM_PIN);
}

void loop() {
  unsigned long timestamp = millis();

  digitalWrite(LED_PIN, 1);
  rfDevice.send(PING_MESSAGE);
  Serial.println("ping sent");
  digitalWrite(LED_PIN, 0);

  // Send a ping every second
  delay(1000 - (timestamp - millis()));
}