#include <iostream>
#include <protocol.h>
#include <sstream>

#include "http/http.h"
#include "HTTPBackend.h"

HTTPBackend::HTTPBackend(const std::string &url) : m_url(url) {
  initHTTP(false);
}

HTTPBackend::~HTTPBackend() {
  releaseHTTP();
}

void HTTPBackend::processDoorSignal(int sensor_id, int sensor_type, uint8_t message) {
  std::ostringstream oss;
  oss << "?sensor_type=" << sensor_type << "&sensor_id=" << sensor_id << "&state=" << (message == 0) ? "closed" : "open";
  post(m_url.c_str(), oss.str().c_str());
}
