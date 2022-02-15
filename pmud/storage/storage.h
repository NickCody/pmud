#pragma once

#include <memory>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>

#include "hiredis/hiredis.h"

#include "pnet/util.h"
#include "logger/logger.h"

namespace primordia::mud::storage {

  using namespace std;

  
  class Storage {
  public:
    virtual ~Storage(){};
    // virtual bool store(const string& key, const string& value);
    // virtual bool store(const string& key, int value);
  };

  class RedisStorage : public Storage {
    const string& m_host;
    int m_port;
    unique_ptr<redisContext> m_connection;
    string env;

  public:
    RedisStorage(const string& host, int port)
        : m_host(host),
          m_port(port) {
    }

    ~RedisStorage() {

    }

    bool init(const string& env) {
      string ip;
      if (hostname_to_ip(m_host, ip) == -1) {
        LOG_INFO("Could not resolve hostname {} to IP address!", m_host);
        return false;
      }

      LOG_INFO("Redis host {} = ip {}", m_host, ip);

      this->env = env;
      m_connection = unique_ptr<redisContext>(redisConnect(ip.c_str(), m_port));
      if (m_connection == nullptr || m_connection->err) {
        if (m_connection) {
          LOG_INFO("Error: {}", m_connection->errstr);
        } else {
          LOG_INFO_1("Can't allocate redis context");
        }
        return false;
      }
      return true;
    }

    // bool store(const string& key, const string& value) override {}
    // bool store(const string& key, int value) override {}

  private:
    int hostname_to_ip(const string& hostname, string& ip) {
      struct hostent* he;
      struct in_addr** addr_list;
      int i;

      if ((he = gethostbyname(hostname.c_str())) == NULL) {
        return -1;
      }

      addr_list = (struct in_addr**)he->h_addr_list;

      char buffer[100];
      for (i = 0; addr_list[i] != NULL; i++) {
        // Return the first one;
        strcpy(buffer, inet_ntoa(*addr_list[i]));
        ip = buffer;
        return 0;
      }

      return -1;
    }
  };

} // namespace primordia::mud::storage