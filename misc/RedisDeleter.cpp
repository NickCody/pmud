#include <memory>
#include <fmt/core.h>
#include <iostream>
#include <sstream>

#include "hiredis/hiredis.h"

using namespace std;

struct RedisReplyDeleter {
  void operator()(void* reply) const {
    fmt::print("Deleting reply object!\n");
    freeReplyObject(reply);
  }
};

struct RedisContextDeleter {
  void operator()(redisContext* context) const {
    fmt::print("Deleting context object!\n");
    redisFree(context);
  }
};

using RedisContextUniquePtr = std::unique_ptr<redisContext, RedisContextDeleter>;
using RedisReplyUniquePtr = std::unique_ptr<void, RedisReplyDeleter>;

int main() {

  RedisContextUniquePtr context(redisConnect("192.168.86.25", 6379));
  if (context == nullptr || context->err) {
    if (context) {
      fmt::print("Error: {}\n", context->errstr);
    } else {
      fmt::print("Can't allocate redis context\n");
    }
    return false;
  }

  fmt::print("Connected to Redis!\n");

  RedisReplyUniquePtr reply(redisCommand(context.get(), "SET foo bar"));
  fmt::print("{}", redis_reply_dump((redisReply*)reply.get()));

  RedisReplyUniquePtr reply2(redisCommand(context.get(), "GET foo"));
  fmt::print("{}", redis_reply_dump((redisReply*)reply2.get()));
  return 0;
}