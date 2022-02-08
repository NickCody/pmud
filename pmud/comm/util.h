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

} // namespace primordia::mud