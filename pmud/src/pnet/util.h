#pragma once
#include "caf/actor.hpp"
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <regex>
#include <optional>

#include <spdlog/spdlog.h>
#include <fmt/format.h>

namespace primordia::mud {

  using namespace std;
  using namespace caf;

  void initialize_sockaddr(const std::string& address, uint16_t port, sockaddr_in& sa) {
    sa.sin_family = AF_INET;
    inet_aton(address.c_str(), &sa.sin_addr);
    sa.sin_port = htons(port);
    memset(&sa.sin_zero, 0, sizeof(sa.sin_zero));
  }

  optional<actor> find_connection_actor(actor_system& sys, int connection) {
    for (auto actor_in_registry : sys.registry().named_actors()) {
      string name = fmt::format("Connection{}", connection);
      if (actor_in_registry.first == name) {
        return actor_cast<actor>(actor_in_registry.second);
      }
    }

    SPDLOG_ERROR("Could not find connection actor for {}", connection);
    return nullopt;
  }

} // namespace primordia::mud