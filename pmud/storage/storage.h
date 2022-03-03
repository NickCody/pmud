#pragma once

#include <memory>
#include <string>
#include <map>
#include <sstream>
#include <netdb.h>
#include <arpa/inet.h>
#include <limits.h>
#include <ctime>
#include "hiredis/hiredis.h"

#include "spdlog/spdlog.h"

namespace primordia::mud::storage {

  using namespace std;

  struct RedisReplyDeleter {
    void operator()(void* reply) const {
      freeReplyObject(reply);
    }
  };

  struct RedisContextDeleter {
    void operator()(redisContext* context) const {
      SPDLOG_INFO("Shutting down redis");
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
    // virtual bool map_store(const string& map_name, const map<string, string> pairs) = 0;
    virtual bool list_store(const string& list_name, const string& value) = 0;
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
        SPDLOG_INFO("Could not resolve hostname {} to IP address!", m_host);
        return false;
      }

      SPDLOG_INFO("Redis host {} = ip {}", m_host, ip);

      m_context = RedisContextUniquePtr(redisConnect(ip.c_str(), m_port));
      if (m_context == nullptr || m_context->err) {
        if (m_context) {
          SPDLOG_INFO("Error: {}", m_context->errstr);
        } else {
          SPDLOG_INFO("Can't allocate redis context");
        }
        return false;
      }

      value_store("server_last_connected", fmt::format("{}", (long)std::time(0)));
      return true;
    }

    RedisReplyUniquePtr command(const string& cmd) {
      spdlog::debug("To redis => {}", cmd);
      auto reply = RedisReplyUniquePtr(redisCommand(m_context.get(), cmd.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command [{}], code {}: {}", cmd, m_context->err, m_context->errstr);
      }
      return reply;
    }

    bool value_store(const string& key, const string& value) override {
      auto _key = _replace_all(key, " ", "_");
      return command(fmt::format(R"(SET {} "{}")", _key, value)) != nullptr;
    }

    bool map_store(const string& map_name, const string& key, const string& value) override {
      auto _map_name = _replace_all(map_name, " ", "_");
      auto _key = _replace_all(key, " ", "_");
      return command(fmt::format(R"(HSET {} {} "{}")", _map_name, _key, value)) != nullptr;
    }

    // bool map_store(const string& map_name, const map<string, string> pairs) override {
    //   auto _map_name = _replace_all(map_name, " ", "_");
    //   ostringstream cmd;
    //   for (auto pair : pairs) {
    //     cmd << _replace_all(pair.first, " ", "_") << " \"" << pair.second << "\" "
    //         << " ";
    //   }

    //   return command(fmt::format(R"(HMSET {} "{}")", _map_name, cmd.str())) != nullptr;
    // }

    bool list_store(const string& list_name, const string& value) override {
      auto _list_name = _replace_all(list_name, " ", "_");
      return command(fmt::format(R"(RPUSH {} "{}")", _list_name, value)) != nullptr;
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

  inline unique_ptr<Storage> initialize_redis_storage() {
    char* e_redis_host = getenv("REDIS_HOST");
    char* e_redis_port = getenv("REDIS_PORT");

    if (!e_redis_host || !e_redis_port) {
      SPDLOG_ERROR("Error, redis host and port should be specified with environment REDIS_HOST and REDIS_PORT");
      return nullptr;
    }

    const string& redis_host = e_redis_host;
    uint16_t redis_port = (uint16_t)stoul(e_redis_port);
    auto storage = make_unique<primordia::mud::storage::RedisStorage>(redis_host, redis_port);

    if (!storage->init()) {
      SPDLOG_ERROR("Error, could not connect to REDIS_HOST {} on REDIS_PORT {}", redis_host, redis_port);
      return nullptr;
    }

    SPDLOG_INFO("Successsfully connected to redis at {}:{}!", redis_host, redis_port);

    return storage;
  }

} // namespace primordia::mud::storage