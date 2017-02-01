#include <regex>
#include "RBackend.h"

using namespace std;
namespace R = RethinkDB;

RBackend::RBackend(const string &url, const string &database, const string &table) : m_url(url) {
  cmatch cm;
  if (!regex_match(url.c_str(), cm, regex("(?:http://)?([^:]+):([0-9]+)")))
    throw runtime_error("Badly formed url:" + url);
  m_conn = R::connect(cm[1], atoi(cm[2].str().c_str()));
  // TODO(JD): Check database and table exist
  R::Cursor dbcursor = R::db_list().run(*m_conn);
  if (!(find_if(begin(dbcursor), end(dbcursor), [&](R::Datum d) { return database == (*d.get_string()); }) != end(dbcursor)))
    throw runtime_error("No such database:" + database);

}

void RBackend::processDoorSignal(int sensor_id, int sensor_type, uint8_t message) {

}
