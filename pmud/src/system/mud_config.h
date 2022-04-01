#pragma once
#include <string>
#include <set>

namespace primordia::mud::system {

  using namespace std;

  struct MudConfig {
    string name;
    string address;
    uint16_t port;
    uint16_t max_queued_connections;
    string log_name;
    string log_level;
    string log_filename;
    bool log_truncate;
  };
} // namespace primordia::mud::system
