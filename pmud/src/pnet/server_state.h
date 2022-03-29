#pragma once
#include <string>
#include <set>
#include <optional>
#include <spdlog/logger.h>

namespace primordia::mud::pnet {

  using namespace std;
  using namespace caf;

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
} // namespace primordia::mud::pnet
