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
#include "storage/redis_types.h"
#include "storage/redis_stream_injestor.h"

namespace primordia::mud::storage {

  using namespace std;

  class Storage {
  public:
    virtual ~Storage(){};
    virtual bool init() = 0;
    virtual bool value_store(const string& key, const string& value) = 0;
    virtual bool map_store(const string& map_name, const string& key, const string& value) = 0;
    virtual bool list_store(const string& list_name, const string& value) = 0;
    virtual bool set_store(const string& set_name, const string& value) = 0;
    virtual bool del_key(const string& key) = 0;
    virtual vector<StreamResponse> read_stream_block(const string& stream_name, const string& pos) = 0;
    virtual string raw(const string& command) = 0;
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
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "SET %s %s", key.c_str(), value.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command value_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    bool map_store(const string& map_name, const string& key, const string& value) override {
      spdlog::debug("redis: HSET {} {} {}", map_name, key, value);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "HSET %s %s %s", map_name.c_str(), key.c_str(), value.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command map_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    bool list_store(const string& list_name, const string& value) override {
      spdlog::debug("redis: RPUSH {} {}", list_name, value);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "RPUSH %s %s", list_name.c_str(), value.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command list_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    bool set_store(const string& set_name, const string& value) override {
      spdlog::debug("redis: SADD {} {}", set_name, value);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "SADD %s %s", set_name.c_str(), value.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command set_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    bool del_key(const string& key) override {
      spdlog::debug("redis: DEL {}", key);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "DEL %s", key.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command del_key: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    vector<StreamResponse> read_stream_block(const string& stream_name, const string& pos) override {

      spdlog::debug("redis: XREAD BLOCK 0 STREAMS {} {}", stream_name, pos);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "XREAD BLOCK 0 STREAMS %s %s", stream_name.c_str(), pos.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command del_key: code {}: {}", m_context->err, m_context->errstr);
        return vector<StreamResponse>();
      } else {
        return construct_streams(reply.get());
      }
    }

    string raw(const string& command) override {
      spdlog::debug("redis (raw):{}", command);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), command.c_str()));
      if (!reply) {
        SPDLOG_ERROR("Error running command del_key: code {}: {}", m_context->err, m_context->errstr);
        return "";
      } else {
        return redis_reply_dump(reply.get());
      }
    }

  private:
    string redis_reply_dump(redisReply* reply, const string& indent = "") {

      ostringstream str;

      switch (reply->type) {
      case REDIS_REPLY_STATUS:
        return fmt::format("{}REDIS_REPLY_STATUS: {}\n", indent, reply->str);
      case REDIS_REPLY_ERROR:
        return fmt::format("{}REDIS_REPLY_ERROR: {}\n", indent, reply->str);
      case REDIS_REPLY_INTEGER:
        return fmt::format("{}REDIS_REPLY_INTEGER: {}\n", indent, reply->integer);
      case REDIS_REPLY_NIL:
        return fmt::format("{}REDIS_REPLY_NIL\n", indent);
      case REDIS_REPLY_STRING:
        return fmt::format("{}REDIS_REPLY_STRING: {}", indent, reply->str);
      case REDIS_REPLY_ARRAY:
        str << fmt::format("{}REDIS_REPLY_ARRAY\n", indent);
        for (size_t i = 0; i < reply->elements; i++) {
          str << redis_reply_dump(reply->element[i], indent + "  ") << endl;
        }
        return str.str();
      default:
        break;
      }

      return "Unknown";
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