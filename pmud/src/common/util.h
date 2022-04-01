#pragma once
#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <regex>
#include <optional>
#include <netdb.h>
#include <arpa/inet.h>

#include <spdlog/spdlog.h>
#include <fmt/format.h>

namespace primordia::mud::common {

  using namespace std;

  /**
   * \brief Returns a hostname from an ip address
   *
   * @param hostname hostname to resolve
   * @param ip The ip address to return
   * @return 0 on succeess, -1 on failure
   *
   */

  int hostname_to_ip(const string& hostname, string& ip) {
    struct sockaddr whereto;
    struct hostent* hp;
    struct sockaddr_in* to;

    memset(&whereto, 0, sizeof(struct sockaddr));
    to = (struct sockaddr_in*)&whereto;
    to->sin_family = AF_INET;
    to->sin_addr.s_addr = inet_addr(hostname.c_str());
    if (to->sin_addr.s_addr != (in_addr_t)(-1)) {
      ip = hostname;
    } else {
      hp = gethostbyname(hostname.c_str());
      if (!hp) {
        SPDLOG_WARN("Unknown host {}", hostname);
        return -1;
      }
      to->sin_family = hp->h_addrtype;
      memcpy(&(to->sin_addr.s_addr), hp->h_addr, hp->h_length);
      ip = hp->h_name;
    }
    return 0;
  }

  void initialize_sockaddr(const string& address, uint16_t port, sockaddr_in& sa) {
    sa.sin_family = AF_INET;
    inet_aton(address.c_str(), &sa.sin_addr);
    sa.sin_port = htons(port);
    memset(&sa.sin_zero, 0, sizeof(sa.sin_zero));
  }

} // namespace primordia::mud::common