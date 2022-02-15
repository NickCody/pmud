#include <memory>
#include <fmt/format.h>
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

string redis_reply_dump(redisReply* reply) {

  ostringstream str;

  switch (reply->type) {
  case REDIS_REPLY_STATUS:
    return fmt::format("REDIS_REPLY_STATUS: {}\n", reply->str);
  case REDIS_REPLY_ERROR:
    return fmt::format("REDIS_REPLY_ERROR: {}\n", reply->str);
  case REDIS_REPLY_INTEGER:
    return fmt::format("REDIS_REPLY_INTEGER: {}\n", reply->integer);
  case REDIS_REPLY_NIL:
    return fmt::format("REDIS_REPLY_NIL\n");
  case REDIS_REPLY_STRING:
    return fmt::format("REDIS_REPLY_STRING: {}\n", reply->str);
  case REDIS_REPLY_ARRAY:
    str << fmt::format("REDIS_REPLY_ARRAY") << endl;
    for (size_t i = 0; i < reply->elements; i++) {
      str << redis_reply_dump(reply->element[i]) << endl;
    }
    return str.str();
  default:
    break;
  }

  return "Unknown";
}

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