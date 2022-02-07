#pragma once

#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace primordia::mud {

  void initialize_sockaddr(const std::string& address, uint16_t port, sockaddr_in& sa) {
    sa.sin_family = AF_INET;
    inet_aton(address.c_str(), &sa.sin_addr);
    sa.sin_port = htons(port);
    memset(&sa.sin_zero, 0, sizeof(sa.sin_zero));
  }

  string LOG_INFO() {
    auto now = std::chrono::system_clock::now();
    auto tt = std::chrono::system_clock::to_time_t(now);
    std::ostringstream ss;
    ss << std::put_time(gmtime(&tt), "%F %T");
    ss << " INFO ";
    return ss.str();
  }

} // namespace primordia::mud