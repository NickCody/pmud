#pragma once

#include <memory>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>
// #include <limits.h>
// #include <ctime>
// #include "hiredis/hiredis.h"

#include "spdlog/spdlog.h"

namespace primordia::mud::common {

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

} // namespace primordia::mud::common
