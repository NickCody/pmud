#pragma once

#include <memory>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <limits.h>
#include <ctime>
#include "hiredis/hiredis.h"

#include "pnet/util.h"
#include "logger/logger.h"

namespace primordia::mud::storage {

  using namespace std;

  struct RedisReplyDeleter {
    void operator()(void* reply) const {
      freeReplyObject(reply);
    }
  };

  struct RedisContextDeleter {
    void operator()(redisContext* context) const {
      LOG_INFO_1("Shutting down redis");
      redisFree(context);
    }
  };

  using RedisContextUniquePtr = std::unique_ptr<redisContext, RedisContextDeleter>;
  using RedisReplyUniquePtr = std::unique_ptr<void, RedisReplyDeleter>;

  class Storage {
  public:
    virtual ~Storage(){};
    virtual bool init() = 0;
    virtual bool value_store(const string& key, const string& value) = 0;
    virtual bool map_store(const string& map_name, const string& key, const string& value) = 0;
    virtual bool map_store(const string& map_name, const map<string, string> pairs) = 0;
  };

  class RedisStorage : public Storage {
    const string& m_host;
    int m_port;
    RedisContextUniquePtr m_context;

  public:
    RedisStorage(const string& host, int port)
        : m_host(host),
          m_port(port) {}

    ~RedisStorage() {}

    bool init() {
      string ip;
      if (hostname_to_ip(m_host, ip) == -1) {
        LOG_INFO("Could not resolve hostname {} to IP address!", m_host);
        return false;
      }

      LOG_INFO("Redis host {} = ip {}", m_host, ip);

      m_context = RedisContextUniquePtr(redisConnect(ip.c_str(), m_port));
      if (m_context == nullptr || m_context->err) {
        if (m_context) {
          LOG_INFO("Error: {}", m_context->errstr);
        } else {
          LOG_INFO_1("Can't allocate redis context");
        }
        return false;
      }

      value_store("server_last_connected", fmt::format("{}", (long)std::time(0)));
      return true;
    }

    RedisReplyUniquePtr command(const string& cmd) {
      LOG_DEBUG("Sending [{}] to redis...", cmd);
      auto reply = RedisReplyUniquePtr(redisCommand(m_context.get(), cmd.c_str()));
      if (!reply) {
        LOG_ERROR("Error running command [{}], code {}: {}", cmd, m_context->err, m_context->errstr);
      }
      return reply;
    }

    bool value_store(const string& key, const string& value) override {
      return command(fmt::format("SET {} {}", key, value)) != nullptr;
    }

    bool map_store(const string& map_name, const string& key, const string& value) override {
      return command(fmt::format("HSET {} {} {}", map_name, key, value)) != nullptr;
    }

    bool map_store(const string& map_name, const map<string, string> pairs) override {
      ostringstream cmd;
      for (auto pair : pairs) {
        cmd << _replace_all(pair.first, " ", "_") << " \"" << pair.second << "\" "
            << " ";
      }

      return command(fmt::format("HMSET {} {}", map_name, cmd.str())) != nullptr;
    }

  private:
    int hostname_to_ip(const string& hostname, string& ip) {
      struct hostent* he;
      struct in_addr** addr_list;
      int i;

      if ((he = gethostbyname(hostname.c_str())) == NULL) {
        return -1;
      }

      addr_list = (struct in_addr**)he->h_addr_list;

      char buffer[PATH_MAX];
      for (i = 0; addr_list[i] != NULL; i++) {
        // Return the first one;
        strcpy(buffer, inet_ntoa(*addr_list[i]));
        ip = buffer;
        return 0;
      }

      return -1;
    }

    string _replace_all(const string& text, const string& from, const string& to) {
      string replaced = text;
      for (auto at = replaced.find(from, 0); at != std::string::npos; at = replaced.find(from, at + to.length())) {
        replaced.replace(at, from.length(), to);
      }
      return replaced;
    }
  };

} // namespace primordia::mud::storage