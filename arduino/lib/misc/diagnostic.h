#ifndef __DIAGNOSTIC_H__
#define __DIAGNOSTIC_H__

#include "voltmeter.h"

void print_diagnostic(int sensor_type, int sendor_id) {
  Serial.println("");
  Serial.println("*** DIAG report ***");
  Serial.print("type of device: ");
  Serial.println(DOOR_SENSOR);
  Serial.print("device id: ");
  Serial.println(g_device_number);
  Serial.print("battery level: ");
  Serial.print(readVcc());
  Serial.println(" mV");
  Serial.print("switch status: ");
  Serial.println(digitalRead(MAGNETIC_SWITCH_PIN) == HIGH);
  Serial.println("*******************");
  delay(100);
}

void ask_for_device_number() {
  Serial.println(" Enter a new devide id (press enter for no change):");
  delay(100);
  while (Serial.available() == 0)
    /* just wait */ ;
  String new_device_number = Serial.readStringUntil(13);
  if (new_device_number) {
    new_device_number.trim();
    if (new_device_number.length() != 0) {
      g_device_number = new_device_number.toInt();
      EEPROM.write(EE_DEVICE_NUMBER, g_device_number);
      Serial.println("New sensor id: ");
      Serial.println(g_device_number);
    }
  }
}

#endif //  __DIAGNOSTIC_H__