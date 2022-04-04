#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>
#include <sstream>
#include <netdb.h>
#include <arpa/inet.h>
#include <limits.h>
#include <ctime>
#include <spdlog/spdlog.h>
#include <hiredis/hiredis.h>

#include "common/util.h"
#include "storage/storage.h"
#include "storage/redis_types.h"
#include "storage/redis_stream_injestor.h"

namespace primordia::mud::storage::redis {

  using namespace std;

  class RedisStorage : public Storage {
    const string& m_host;
    int m_port;
    RedisContextUniquePtr m_context;

  public:
    RedisStorage(const string& host, int port)
        : m_host(host),
          m_port(port) {}

    ~RedisStorage() {}

    bool init() override {
      string ip;
      if (primordia::mud::common::hostname_to_ip(m_host, ip) == -1) {
        spdlog::info("Could not resolve hostname {} to IP address!", m_host);
        return false;
      }

      spdlog::info("Redis host {} = ip {}", m_host, ip);

      m_context = RedisContextUniquePtr(redisConnect(ip.c_str(), m_port));
      if (m_context == nullptr || m_context->err) {
        if (m_context) {
          spdlog::info("Error: {}", m_context->errstr);
        } else {
          spdlog::info("Can't allocate redis context");
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
        spdlog::error("Error running command value_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    virtual std::optional<string> value_get(const string& key) override {
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "GET %s", key.c_str()));
      if (!reply) {
        spdlog::error("Error running command set_store: code {}: {}", m_context->err, m_context->errstr);
        return nullopt;
      }
      if (reply->type == REDIS_REPLY_STRING) {
        return reply->str;
      } else {
        spdlog::error("Error running command GET, return type {}, value {}", reply->type, reply->str);
        return nullopt;
      }
    }

    bool map_store(const string& map_name, const string& key, const string& value) override {
      spdlog::debug("redis: HSET {} {} {}", map_name, key, value);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "HSET %s %s %s", map_name.c_str(), key.c_str(), value.c_str()));
      if (!reply) {
        spdlog::error("Error running command map_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    virtual std::optional<kv_t> map_get(const string map_name) override {
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "HGETALL %s", map_name.c_str()));
      if (!reply) {
        spdlog::error("Error running command set_store: code {}: {}", m_context->err, m_context->errstr);
        return nullopt;
      }
      if (reply->type == REDIS_REPLY_MAP || reply->type == REDIS_REPLY_ARRAY) {
        kv_t result;
        for (size_t i = 0; i < reply->elements; i += 2) {
          result[reply->element[i]->str] = reply->element[i + 1]->str;
        }
        return result;
      } else {
        spdlog::error("Error, didn't get REDIS_REPLY_MAP, got {}", reply->type);

        return nullopt;
      }
    }

    bool list_store(const string& list_name, const string& value) override {
      spdlog::debug("redis: RPUSH {} {}", list_name, value);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "RPUSH %s %s", list_name.c_str(), value.c_str()));
      if (!reply) {
        spdlog::error("Error running command list_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    virtual std::optional<list_t> list_get(const string& list_name) override {
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "LRANGE %s 0 -1", list_name.c_str()));
      if (!reply) {
        spdlog::error("Error running command set_store: code {}: {}", m_context->err, m_context->errstr);
        return nullopt;
      }
      if (reply->type == REDIS_REPLY_ARRAY) {
        list_t result;
        for (size_t i = 0; i < reply->elements; i++) {
          result.push_back(reply->element[i]->str);
        }
        return result;
      } else {
        return nullopt;
      }
    }

    bool set_store(const string& set_name, const string& value) override {
      spdlog::debug("redis: SADD {} {}", set_name, value);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "SADD %s %s", set_name.c_str(), value.c_str()));
      if (!reply) {
        spdlog::error("Error running command set_store: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    virtual std::optional<set_t> set_get(const string& set_name) override {
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "SMEMBERS %s", set_name.c_str()));
      if (!reply) {
        spdlog::error("Error running command set_store: code {}: {}", m_context->err, m_context->errstr);
        return nullopt;
      }
      if (reply->type == REDIS_REPLY_ARRAY) {
        set_t result;
        for (size_t i = 0; i < reply->elements; i++) {
          result.insert(reply->element[i]->str);
        }
        return result;
      } else {
        return nullopt;
      }
    }

    bool del_key(const string& key) override {
      spdlog::debug("redis: DEL {}", key);
      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), "DEL %s", key.c_str()));

      if (!reply) {
        spdlog::error("Error running command del_key: code {}: {}", m_context->err, m_context->errstr);
      }
      return reply != nullptr;
    }

    bool event_store(const string& event_name, const StreamRecordFields_t& fields) override {
      vector<string> arg_vector;
      arg_vector.push_back("FCALL");
      arg_vector.push_back(event_name);
      arg_vector.push_back("1");
      arg_vector.push_back("events");
      for (auto const& [key, val] : fields) {
        arg_vector.push_back(key);
        arg_vector.push_back(val);
      }

      stringstream invoc;
      for (const auto& arg : arg_vector) {
        invoc << arg << " ";
      }
      spdlog::debug("event_store: {}", invoc.str());

      std::vector<const char*> argv(arg_vector.size());
      std::transform(arg_vector.begin(), arg_vector.end(), argv.begin(), [](string& str) { return str.c_str(); });

      auto reply = RedisReplyUniquePtr((redisReply*)redisCommandArgv(m_context.get(), arg_vector.size(), argv.data(), NULL));
      if (!reply) {
        spdlog::error("Error running command event_store: code {}: {}", m_context->err, m_context->errstr);
      }

      return reply != nullptr;
    }

    bool stream_store(const string& map_name, const StreamRecordFields_t& fields) override {

      vector<string> arg_vector;
      arg_vector.push_back("XADD");
      arg_vector.push_back(map_name);
      arg_vector.push_back("*");
      for (auto const& [key, val] : fields) {
        arg_vector.push_back(key);
        arg_vector.push_back(val);
      }

      std::vector<const char*> argv(arg_vector.size());
      std::transform(arg_vector.begin(), arg_vector.end(), argv.begin(), [](string& str) { return str.c_str(); });

      auto reply = RedisReplyUniquePtr((redisReply*)redisCommandArgv(m_context.get(), arg_vector.size(), argv.data(), NULL));
      if (!reply) {
        spdlog::error("Error running command stream_store: code {}: {}", m_context->err, m_context->errstr);
      }

      return reply != nullptr;
    }

    vector<StreamResponse> read_stream_raw(const string& command) override {
      spdlog::debug("redis:{}", command);

      auto reply = RedisReplyUniquePtr((redisReply*)redisCommand(m_context.get(), command.c_str()));

      if (!reply) {
        spdlog::error("Error running command del_key: code {}: {}", m_context->err, m_context->errstr);
        return vector<StreamResponse>();
      } else {
        return construct_stream_responses(*reply.get());
      }
    }

    vector<StreamResponse> read_stream_block(const string& stream_name, const string& pos, uint32_t timeout) override {
      auto _timeout = std::to_string(timeout);
      spdlog::debug("redis: XREAD BLOCK {} STREAMS {} {}", _timeout, stream_name, pos);

      auto reply = RedisReplyUniquePtr(
          (redisReply*)redisCommand(m_context.get(), "XREAD BLOCK %s STREAMS %s %s", _timeout.c_str(), stream_name.c_str(), pos.c_str()));

      if (!reply) {
        spdlog::error("Error running command del_key: code {}: {}", m_context->err, m_context->errstr);
        return vector<StreamResponse>();
      } else {
        return construct_stream_responses(*reply.get());
      }
    }

    unique_ptr<redisReply> raw(const string& command) {
      spdlog::debug("redis (raw):{}", command);

      redisReply* reply = (redisReply*)redisCommand(m_context.get(), command.c_str());

      if (!reply) {
        spdlog::error("Error running command del_key: code {}: {}", m_context->err, m_context->errstr);
      }

      return unique_ptr<redisReply>(reply);
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
    auto storage = make_unique<RedisStorage>(redis_host, redis_port);

    if (!storage->init()) {
      spdlog::error("Error, could not connect to REDIS_HOST {} on REDIS_PORT {}", redis_host, redis_port);
      return nullptr;
    }

    spdlog::info("Successsfully connected to redis at {}:{}!", redis_host, redis_port);

    return storage;
  }

} // namespace primordia::mud::storage::redis