#ifndef __SENSOR_TYPE_H__
#define __SENSOR_TYPE_H__

#define DOOR_SENSOR     5
#define THERMOMETER     6
#define HUMIDITY_SENSOR 7
#define MOTION_SENSOR   8
#define CAMERA          9
#define LOCK            10
#define KEYPAD          11

#define GET_RESOURCE_NAME(type) \
  (type == DOOR_SENSOR) ? "door-sensors" : \
  (type == THERMOMETER) ? "thermometers" : \
  (type == HUMIDITY_SENSOR) ? "humidity-sensors" : \
  (type == MOTION_SENSOR) ? "motion-sensors" : \
  (type == CAMERA) ? "cameras" : \
  (type == LOCK) ? "locks" : \
  (type == KEYPAD) ? "keypads" : "unknowns"


#endif //  __SENSOR_TYPE_H__
