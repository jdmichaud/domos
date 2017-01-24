#ifndef __DIAGNOSTIC_H__
#define __DIAGNOSTIC_H__

#include "voltmeter.h"

void print_diagnostic(int sensor_type, int sendor_id) {
  Serial.println("");
  Serial.println("*** DIAG report ***");
  Serial.print("type of device: ");
  Serial.println(sensor_type);
  Serial.print("device id: ");
  Serial.println(sendor_id);
  Serial.print("battery level: ");
  Serial.print(readVcc());
  Serial.println(" mV");
  Serial.println("*******************");
  delay(100);
}

void ask_for_device_number(int device_number_addr, int *device_number) {
  Serial.print(" Enter a new device id (press enter for no change): ");
  delay(100);
  while (Serial.available() == 0)
    /* just wait */ ;
  String new_device_number = Serial.readStringUntil(13);
  if (new_device_number) {
    new_device_number.trim();
    if (new_device_number.length() != 0) {
      *device_number = new_device_number.toInt();
      EEPROM.write(device_number_addr, *device_number);
      Serial.print("New sensor id: ");
      Serial.println(*device_number);
      delay(100);
    }
  }
}

#endif //  __DIAGNOSTIC_H__
