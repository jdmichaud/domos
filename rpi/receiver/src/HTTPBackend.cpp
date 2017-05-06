#include <iostream>
#include <protocol.h>
#include <sstream>

#include "http.h"
#include "sensor_types.h"
#include "HTTPBackend.h"

HTTPBackend::HTTPBackend(const std::string &url) : m_url(url) {
  initHTTP(false);
}

HTTPBackend::~HTTPBackend() {
  releaseHTTP();
}

void HTTPBackend::processDoorSignal(int sensor_id, int sensor_type, uint8_t message) {
  std::ostringstream full_url;
  full_url << m_url << "/" << (GET_RESOURCE_NAME(sensor_type)) << "/";
  std::ostringstream body;
  body << "{"
       << "\"sensorId\":" << sensor_id << ","
       << "\"open\":\""   << (message ? "true" : "false") << "\""
       << "}";
  if (post(full_url.str().c_str(), body.str().c_str()) == HTTP_FAIL) {
    std::cerr << "POST request to backend " << m_url << " failed with " << getLastError() << std::endl;
  }
}
