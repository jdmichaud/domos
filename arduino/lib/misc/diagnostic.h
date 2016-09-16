#ifndef __DIAGNOSTIC_H__
#define __DIAGNOSTIC_H__

#include "voltmeter.h"

void print_diagnostic(int sensor_type, int sendor_id) {
  Serial.println(" *** DIAG report ***");
  Serial.print("type of device: ");
  Serial.println(sensor_type);
  Serial.print("device id: ");
  Serial.println(sendor_id);
  Serial.print("battery level: ");
  Serial.print(readVcc());
  Serial.println(" mV");
  Serial.println(" *******************");
}

void ask_for_device_number() {
  Serial.println(" Enter a new device id (press enter for no change):");
  while (Serial.available() == 0)
    /* just wait */ ;
  int new_device_number = Serial.read();
  if (new_device_number != -1) {
    g_device_number = new_device_number;
    EEPROM.write(EE_DEVICE_NUMBER, g_device_number);
    Serial.println("New sensor id: ");
    Serial.println(g_device_number);
  }
}

#endif //  __DIAGNOSTIC_H__