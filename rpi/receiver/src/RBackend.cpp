#include <regex>
#include "RBackend.h"

namespace R = RethinkDB;

RBackend::RBackend (const std::string &url) : m_url(url) {
  std::cmatch cm;
  if (!std::regex_match(url.c_str(), cm, std::regex("(?:http://)?([^:]+):([0-9]+)")))
    throw std::runtime_error("Badly formed url:" + url);
  m_conn = R::connect(cm[1], atoi(cm[2].str().c_str()));
  // TODO(JD): Check connection worked
  // TODO(JD): Check database and table exist
}

void RBackend::processDoorSignal(int sensor_id, int sensor_type, uint8_t message) {

}
