#include <EEPROM.h>
#include <RCSwitch.h>
#include <LowPower.h>
#include "limits.h"
#include "protocol.h"
#include "sensor_types.h"
#include "voltmeter.h"

RCSwitch rfDevice = RCSwitch();

int SERIAL_COMMUNICATION_SPEED = 9600;
// How long do we sleep. The longer, the better for the battery but the
// resolution of our switch will suffer.
// Usual values: SLEEP_8S or SLEEP_FOREVER
period_t SLEEP_PERIOD = SLEEP_FOREVER;
// Pin on which the magnetic switch is plugged.
int MAGNETIC_SWITCH_PIN = 2;
// Pin on which the diagnostic switch is plugged.
int DIAG_SWITCH_PIN = 3;
// Pin on which the signaling LED is plugged
// The LED will be lit when the door is open or when in DIAG mode
int LED_PIN = 4;
// Pin on which the RF device is plugged;
int RF_DEVICE_COMM_PIN = 13;
int RF_DEVICE_POWER_PIN = 10;
// Status of the magnetic switch. We consider the door closed
boolean g_contact = true;
// EEPROM address usage
int EE_DEVICE_NUMBER = 0;
// device number
int g_device_number;
// Will store last time LED was updated
unsigned long g_previous_millis = 0;
// ledState used to set the LED
int ledState = LOW;

void setupRFDevice() {
  // Transmitter is connected to Arduino Pin #10
  rfDevice.enableTransmit(RF_DEVICE_COMM_PIN);
  // Optional set pulse length.
  // rfDevice.setPulseLength(320);
  // Optional set protocol (default is 1, will work for most outlets)
  // rfDevice.setProtocol(2);
  // Optional set number of transmission repetitions.
  // rfDevice.setRepeatTransmit(15);
}

void sendStatus(boolean open) {
  char codeWord1[33];
  char codeWord2[33];
  packet_t packet;
  // Power up RF device
  digitalWrite(RF_DEVICE_POWER_PIN, HIGH);
  setupRFDevice();
  // Create the communication packet
  uint8_t message = open ? 1 : 0;
  create_packet(DOOR_SENSOR, g_device_number, 1, &message, &packet, NULL);
  codeWord1[32] = '\0';
  codeWord2[32] = '\0';
  for (int packetSize = sizeof (packet_t) * CHAR_BIT; packetSize; --packetSize) {
    if (packetSize <= 32) {
      codeWord1[packetSize - 1] = packet & 1 ? '1' : '0';
      // Serial.print("CodeWord1 ");
      // Serial.print(packetSize);
      // Serial.print(" ");
      // Serial.println(codeWord1[packetSize]);
    }
    else {
      codeWord2[packetSize - 33] = packet & 1 ? '1' : '0';
      // Serial.print("CodeWord2 ");
      // Serial.print(packetSize - 32);
      // Serial.print(" ");
      // Serial.println(codeWord2[packetSize - 32]);
    }
    packet >>= 1;
  }
  Serial.print(g_device_number);
  // Send packet
  rfDevice.send(codeWord1);
  rfDevice.send(codeWord2);
  // Power down RF device
  digitalWrite(RF_DEVICE_POWER_PIN, LOW);
}

// Function called on door state change
void doorStateChange() {
  // Check contact state. We assume a normally closed switch.
  boolean newContact = (digitalRead(MAGNETIC_SWITCH_PIN) == HIGH);
  // Set the signaling LED
  digitalWrite(LED_PIN, newContact ? LOW : HIGH);
  // Does the contact status changed since we last check
  if (newContact != g_contact) {
    // Send the status of the contact on one bit
    sendStatus(newContact);
    // Update the status of the contact
    g_contact = newContact;
  }
}

// Print a diagnostic report on the serial line and ask for a new device number
void diagnotic() {
  // Entering DIAG mode
  digitalWrite(LED_PIN, HIGH);
  Serial.println(" *** DIAG report ***");
  Serial.print("type of device: ");
  Serial.println(DOOR_SENSOR);
  Serial.print("device id: ");
  Serial.println(g_device_number);
  Serial.print("battery level: ");
  Serial.print(readVcc());
  Serial.println(" mV");
  Serial.print("switch status: ");
  Serial.println(digitalRead(MAGNETIC_SWITCH_PIN) == HIGH);
  Serial.println(" *******************");
  Serial.println("");
  Serial.println(" Enter a new devide id (press enter for no change):");
  while (Serial.available() == 0)
    /* just wait */ ;
  int new_device_number = Serial.read();
  if (new_device_number != -1) {
    g_device_number = new_device_number;
    EEPROM.write(EE_DEVICE_NUMBER, g_device_number);
    Serial.println("New sensor id: ");
    Serial.println(g_device_number);
  }
  // Exiting DIAG mode
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  Serial.begin(SERIAL_COMMUNICATION_SPEED);
  /*****************************  Configuration *******************************/
  g_device_number = EEPROM.read(EE_DEVICE_NUMBER);
  if (g_device_number == 255) { // If not setup already, configure it to default value 1
    g_device_number = 1;
    EEPROM.write(EE_DEVICE_NUMBER, g_device_number);
  }
  Serial.print("sensor id: ");
  Serial.println(g_device_number);
  /***************************  Pin initialisation ****************************/
  pinMode(MAGNETIC_SWITCH_PIN, INPUT);
  pinMode(DIAG_SWITCH_PIN, INPUT);
  pinMode(RF_DEVICE_COMM_PIN, OUTPUT);
  pinMode(RF_DEVICE_POWER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, (digitalRead(MAGNETIC_SWITCH_PIN) == HIGH) ? LOW : HIGH);
  /*********************** Interruption initialisation ************************/
  // Allow wake up pin to trigger interrupt on state change.
  attachInterrupt(digitalPinToInterrupt(MAGNETIC_SWITCH_PIN), doorStateChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(DIAG_SWITCH_PIN), diagnotic, RISING);
}

void loop() {
  // Go to sleep mode
  LowPower.powerDown(SLEEP_PERIOD, ADC_OFF, BOD_OFF);
}
