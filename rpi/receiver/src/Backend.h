#ifndef DOMOS_BACKEND_H
#define DOMOS_BACKEND_H

#include <protocol.h>

class Backend {
  virtual void processDoorSignal(int sensor_id, int sensor_type, uint8_t message) = 0;
};


#endif //DOMOS_BACKEND_H
