/*
 * Act as a RF 433 Mhz beacon.
 * Will first send 01010101 once
 * Then will send, every second (or more):
 * 01010101010101010000000000000001
 * 01010101010101010000000000000010
 * 01010101010101010000000000000011
 * etc...
 */
#include <RCSwitch.h>

#define PING_MESSAGE "01010101"

int SERIAL_COMMUNICATION_SPEED = 9600;
// Pin on which the RF device is plugged;
int RF_DEVICE_COMM_PIN = 13;
// Pin on which the RF device is plugged;
int LED_PIN = 9;

// sizeof(int) * 8;
#define INT_SIZE_IN_BITS 16

RCSwitch rfDevice = RCSwitch();

// buffer must have length >= sizeof(int) + 1
// Write to the buffer backwards so that the binary representation
// is in the correct order i.e.  the LSB is on the far right
// instead of the far left of the printed string
char *int2bin(int a) {
  static char buffer[INT_SIZE_IN_BITS * 2 + 1];
  memset(buffer, '0', INT_SIZE_IN_BITS * 2);
  buffer[32] = 0;
  // Pad 0x5555 at the beginning of the string to improve reception
  buffer[1] = '1';
  buffer[3] = '1';
  buffer[5] = '1';
  buffer[7] = '1';
  buffer[9] = '1';
  buffer[11] = '1';
  buffer[13] = '1';
  buffer[15] = '1';

  for (int i = INT_SIZE_IN_BITS; i > 0; i--) {
    buffer[(INT_SIZE_IN_BITS - 1) + i] = (a & 1) ? '1' : '0';

    a >>= 1;
  }

  return buffer;
}

void setup() {
  Serial.begin(SERIAL_COMMUNICATION_SPEED);
  pinMode(RF_DEVICE_COMM_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  rfDevice.enableTransmit(RF_DEVICE_COMM_PIN);
  rfDevice.setRepeatTransmit(30);
  // To check connection is OK, send a PING we know works
  rfDevice.send(PING_MESSAGE);
}

void loop() {
  static unsigned int counter = 0;

  unsigned long timestamp = millis();

  digitalWrite(LED_PIN, 1);
  // The transmission with this signal is very poor.
  //rfDevice.send(int2bin(counter));
  // The transmission with this signal is good. Keep it as standard configuration.
  rfDevice.send(PING_MESSAGE);
  Serial.println(int2bin(counter));
  digitalWrite(LED_PIN, 0);

  ++counter;

  // Send a ping every second
  int d = 1000 - (timestamp - millis());
  delay(d > 0 ? d : 0);
}