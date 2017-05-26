#ifndef __SENSOR_TYPE_H__
#define __SENSOR_TYPE_H__

#define DOOR_SENSOR     1
#define THERMOMETER     2
#define HUMIDITY_SENSOR 3
#define MOTION_SENSOR   4
#define CAMERA          5
#define LOCK            6
#define KEYPAD          7

#define GET_RESOURCE_NAME(type) \
  (type == DOOR_SENSOR) ? "door-sensors" : \
  (type == THERMOMETER) ? "thermometers" : \
  (type == HUMIDITY_SENSOR) ? "humidity-sensors" : \
  (type == MOTION_SENSOR) ? "motion-sensors" : \
  (type == CAMERA) ? "cameras" : \
  (type == LOCK) ? "locks" : \
  (type == KEYPAD) ? "keypads" : "unknowns"


#endif //  __SENSOR_TYPE_H__
