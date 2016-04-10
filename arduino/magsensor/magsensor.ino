#include <RCSwitch.h>
#include <LowPower.h>

//RCSwitch rfDevice = RCSwitch();

int SERIAL_COMMUNICATION_SPEED = 9600;  
// How long do we sleep. The longer, the better for the battery but the
// resolution of our switch will suffer.
period_t SLEEP_PERIOD = SLEEP_8S;
// Pin on which the magnetic swith is plugged.
int MAGNETIC_SWITCH_PIN = 2;
// Pin on which the RF device is plugged;
int RF_DEVICE_PIN = 13;
// Status of the magnetic switch. We consider the door closed
boolean contact = true;
// Did we just booted ?
boolean justBooted = true;

// Function called on interrupt
void wakeUp() {
  // It is a critical section so don't do anything
}

void flashLED() {
  digitalWrite(RF_DEVICE_PIN, HIGH);
  delay(50);
  digitalWrite(RF_DEVICE_PIN, LOW);    
}

void setup() {
  /***********************  RF Device initialisation **************************/
  Serial.begin(SERIAL_COMMUNICATION_SPEED);
  // Transmitter is connected to Arduino Pin #10
  //rfDevice.enableTransmit(RF_DEVICE_PIN);
  // Optional set pulse length.
  // rfDevice.setPulseLength(320);
  // Optional set protocol (default is 1, will work for most outlets)
  // rfDevice.setProtocol(2);
  // Optional set number of transmission repetitions.
  // rfDevice.setRepeatTransmit(15);
  /********************  Magnetic switch initialisation ***********************/
  pinMode(MAGNETIC_SWITCH_PIN, INPUT);
  pinMode(RF_DEVICE_PIN, OUTPUT);
}

void loop() {
  // Allow wake up pin to trigger interrupt on state change.
  //attachInterrupt(digitalPinToInterrupt(MAGNETIC_SWITCH_PIN), wakeUp, CHANGE);

  // Go to sleep mode
  LowPower.powerDown(SLEEP_PERIOD, ADC_OFF, BOD_OFF);

  // Disable external pin interrupt on wake up pin.
  //detachInterrupt(digitalPinToInterrupt(MAGNETIC_SWITCH_PIN));
    
  // Check contact state. We assume a normally closed switch.
  boolean newContact = (digitalRead(MAGNETIC_SWITCH_PIN) == HIGH);
  // Does the contact status changed since we last check
  if (newContact != contact) {
    // Send the status of the contact on one bit
    //rfDevice.send(contact, 1);
    flashLED();
    // Update the status of the contact
    contact = newContact;
    //Serial.print("pushing to RF device: ");
    //Serial.println(contact);
  }
}
