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

#include "common/pmud_net.h"

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
    virtual bool list_store(const string& list_name, const string& value) = 0;
    virtual bool set_store(const string& set_name, const string& value) = 0;
    virtual bool del_key(const string& key) = 0;
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
      if (primordia::mud::common::hostname_to_ip(m_host, ip) == -1) {
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

    bool value_store(const string& key, const string& value) override {
      spdlog::debug("redis: SET {} {}", key, value);
      auto reply = RedisReplyUniquePtr(redisCommand(m_context.get(), "SET %s %s", key.c_str(), value.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command value_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    bool map_store(const string& map_name, const string& key, const string& value) override {
      spdlog::debug("redis: HSET {} {} {}", map_name, key, value);
      auto reply = RedisReplyUniquePtr(redisCommand(m_context.get(), "HSET %s %s %s", map_name.c_str(), key.c_str(), value.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command map_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    bool list_store(const string& list_name, const string& value) override {
      spdlog::debug("redis: RPUSH {} {}", list_name, value);
      auto reply = RedisReplyUniquePtr(redisCommand(m_context.get(), "RPUSH %s %s", list_name.c_str(), value.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command list_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    bool set_store(const string& set_name, const string& value) override {
      spdlog::debug("redis: SADD {} {}", set_name, value);
      auto reply = RedisReplyUniquePtr(redisCommand(m_context.get(), "SADD %s %s", set_name.c_str(), value.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command set_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    bool del_key(const string& key) override {
      spdlog::debug("redis: DEL {}", key);
      auto reply = RedisReplyUniquePtr(redisCommand(m_context.get(), "DEL %s", key.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command del_key: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }
  };

  inline unique_ptr<Storage> initialize_redis_storage() {
    char* e_redis_host = getenv("REDIS_HOST");
    char* e_redis_port = getenv("REDIS_PORT");

    const string& redis_host = e_redis_host ? e_redis_host : "localhost";
    uint16_t redis_port = e_redis_port ? (uint16_t)stoul(e_redis_port) : 6379;
    auto storage = make_unique<primordia::mud::storage::RedisStorage>(redis_host, redis_port);

    if (!storage->init()) {
      SPDLOG_ERROR("Error, could not connect to REDIS_HOST {} on REDIS_PORT {}", redis_host, redis_port);
      return nullptr;
    }

    SPDLOG_INFO("Successsfully connected to redis at {}:{}!", redis_host, redis_port);

    return storage;
  }

} // namespace primordia::mud::storage