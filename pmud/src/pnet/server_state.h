#pragma once
#include <string>
#include <set>
#include <optional>

namespace primordia::mud::pnet {

  using namespace caf;

  struct MudConfig {
    std::string name;
    std::string address;
    uint16_t port;
    uint16_t max_queued_connections;
  };

} // namespace primordia::mud::pnet
