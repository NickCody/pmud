#include <chrono>
#include <cstdlib>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include "storage/redis_storage.h"

using namespace std;
using namespace primordia::mud::storage;
using namespace primordia::mud::common;

namespace redis_storage = primordia::mud::storage::redis;

void usage() {
  fmt::print("Usage: redis-storage [-k key]\n\n");
  fmt::print("NOTE:\n    REDIS_HOST and REDIS_PORT should be set in environment.\n\n");
}

int main(int argc, char** argv) {
  spdlog::set_level(spdlog::level::off);

  string key;

  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "k:h")) != -1) {
    switch (c) {
    case 'k':
      key = optarg;
      break;
    case 'h':
      usage();
      return EXIT_SUCCESS;
    default:
      usage();
      return EXIT_FAILURE;
    }
  }

  if (key.empty()) {
    spdlog::error("Error, key name must be specified with -k");
    return EXIT_FAILURE;
  }

  auto storage = redis_storage::initialize_redis_storage();

  if (!storage) {
    spdlog::error("Failed to initialize redis storage.");
    return EXIT_FAILURE;
  }

  string key_type;
  {
    redis_storage::RedisStorage* redis = dynamic_cast<redis_storage::RedisStorage*>(storage.get());
    unique_ptr<redisReply> reply = redis->raw(fmt::format("TYPE {}", key));
    if (!reply) {
      fmt::print(stderr, "Could not retrieve key {}, aborting!\n", key);
      return EXIT_FAILURE;
    }
    key_type = reply->str;
  }

  if (key_type == "string") {
    auto result = storage->value_get(key);

    if (!result) {
      fmt::print("Error, could not get key {}\n", key);
      return EXIT_FAILURE;
    }

    fmt::print("{}\n", result.value());
  } else if (key_type == "list") {
    auto result = storage->list_get(key);

    if (!result) {
      fmt::print("Error, could not get key {}\n", key);
      return EXIT_FAILURE;
    }

    for (const auto& val : result.value()) {
      fmt::print("{}\n", val);
    }

  } else if (key_type == "hash") {
    auto result = storage->map_get(key);

    if (!result) {
      fmt::print("Error, could not get hash key {}\n", key);
      return EXIT_FAILURE;
    }

    for (const auto& val : result.value()) {
      fmt::print("{}={}\n", val.first, val.second);
    }
  } else if (key_type == "set") {
    auto result = storage->set_get(key);

    if (!result) {
      fmt::print("Error, could not get key {}\n", key);
      return EXIT_FAILURE;
    }

    for (const auto& val : result.value()) {
      fmt::print("{}\n", val);
    }
  } else {
    fmt::print(stderr, "Error, unsupported type: {}\n", key_type);
  }

  return EXIT_SUCCESS;
}