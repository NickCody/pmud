#pragma once

#include <map>
#include <string>
#include <memory>
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>

namespace primordia::mud::storage::redis {

  using namespace std;

  struct RedisReplyDeleter {
    void operator()(void* reply) const {
      freeReplyObject(reply);
    }
  };

  struct RedisContextDeleter {
    void operator()(redisContext* context) const {
      spdlog::info("Shutting down redis connection");
      redisFree(context);
    }
  };

  using RedisContextUniquePtr = std::unique_ptr<redisContext, RedisContextDeleter>;
  using RedisReplyUniquePtr = std::unique_ptr<redisReply, RedisReplyDeleter>;

} // namespace primordia::mud::storage::redis