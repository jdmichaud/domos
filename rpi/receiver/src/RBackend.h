#ifndef DOMOS_RBACKEND_H
#define DOMOS_RBACKEND_H

#include <stdio.h> // For rethinkdb.h...
#include <string>
#include <rethinkdb.h>
#include "Backend.h"

namespace R = RethinkDB;

class RBackend : public Backend {
public:
  RBackend (const std::string &url);

  virtual void processDoorSignal(int sensor_id, int sensor_type, uint8_t message) override;

private:
  std::string m_url;
  std::unique_ptr<R::Connection> m_conn;
};


#endif //DOMOS_RBACKEND_H
