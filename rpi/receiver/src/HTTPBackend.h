#ifndef DOMOS_HTTPBACKEND_H
#define DOMOS_HTTPBACKEND_H

#include <string>
#include "Backend.h"

class HTTPBackend : public Backend{
public:
  HTTPBackend(const std::string &url);
  virtual ~HTTPBackend();

  void processDoorSignal(int sensor_id, int sensor_type, uint8_t message);

private:
  std::string m_url;

};


#endif //DOMOS_HTTPBACKEND_H
