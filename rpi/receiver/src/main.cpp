#include <getopt.h>
#include <stdint.h>
#include <iostream>
#include <tuple>
#include <protocol.h>
#include "protocol.h"
#include "RF33_adapter.hpp"
#include "HTTPBackend.h"
#include "sensor_types.h"

#define VERSION "0.0.1"

// The RF device data pin
#define RF_DEVICE_COMM_PIN 27
// Default backend URL is localhost
#define DEFAULT_BACKEND_URL "http://127.0.0.1:28015"

#define VERBOSE(s) if (verbose) { s; }

void version() {
  std::cout << "receiver v" << VERSION << " (rfprotocol v" << RFP_VERSION << ")" << std::endl;
  std::cout << "Copyright (C) 2016 Jean-Daniel Michaud." << std::endl;
  std::cout << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>." << std::endl;
  std::cout << "This is free software: you are free to change and redistribute it." << std::endl;
  std::cout << "There is NO WARRANTY, to the extent permitted by law." << std::endl;
  std::cout << std::endl;
  std::cout << "Written by Jean-Daniel Michaud" << std::endl;
}

void help() {
  std::cerr << "Usage: receiver [OPTIONS]" << std::endl;
  std::cerr << std::endl;
  std::cerr << "Options:" << std::endl;
  std::cerr << "  -h, --help                display this help and exit" << std::endl;
  std::cerr << "  -v, --version             display the version and exit" << std::endl;
  std::cerr << "  -V, --verbose             make the program more verbose" << std::endl;
  std::cerr << "      --url url             provide the backend URL to connect to in the form of" << std::endl;
  std::cerr << "                            http://url:port" << std::endl;
  std::cerr << "      --database database   the database name" << std::endl;
  std::cerr << "      --table table         the table name" << std::endl;
}

std::tuple<bool, std::string, std::string, std::string>
parse_options(int argc, char * const argv[]) {
  struct option longopts[] = {
    { "version",  no_argument,        nullptr, 'v' },
    { "help",     no_argument,        nullptr, 'h' },
    { "verbose",  no_argument,        nullptr, 'V' },
    { "url",      required_argument,  nullptr, 'u' },
    { "database", required_argument,  nullptr, 'd' },
    { "table",    required_argument,  nullptr, 't' },
    { 0, 0, 0, 0 },
  };

  int ret = 0;
  int verbose = 0;
  std::string backend_url;
  std::string database_name;
  std::string table_name;
  while ((ret = getopt_long(argc, argv, "vhV", longopts, NULL)) != -1) {
    switch (ret) {
      case 'v':
        version();
        exit(0);
      case 'h':
        help();
        exit(0);
      case 'V':
        verbose = true;
        break;
      case 'u':
        backend_url = argv[optind - 1];
        break;
      case 'd':
        database_name = argv[optind - 1];
        break;
      case 't':
        table_name = argv[optind - 1];
        break;
    }
  }
  return std::make_tuple(verbose, backend_url, database_name, table_name);
}

int main(int argc, char * const argv[]) {
  bool verbose;
  std::string backend_url;
  std::string database_name;
  std::string table_name;
  std::tie(verbose, backend_url, database_name, table_name) = parse_options(argc, argv);
  VERBOSE(std::cout << "Verbose mode activated" << std::endl);
  if (backend_url == "") {
    backend_url = DEFAULT_BACKEND_URL;
  }
  std::cout << "Pushing to " << backend_url << std::endl;

  // The object used to send the POST request
  HTTPBackend httpBackend(backend_url);

  // The object listening to the radio
  RF33Adapter rf33;
  int retcode;
  if ((retcode = rf33.init(RF_DEVICE_COMM_PIN)) != 0) {
    std::cerr << "RF33 initialization failed with " << retcode << std::endl;
    return retcode;
  }
  rf33.receiveMessage([&](const packet_s &packet) {
    if (packet.stype == DOOR_SENSOR) {
      httpBackend.processDoorSignal(packet.sid, packet.stype, packet.message);
    } else {
      std::cerr << "Unknown sensor type: " << packet.stype << std::endl;
    }
  });

  return 0;
}



