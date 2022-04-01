#include <chrono>

#include <spdlog/spdlog.h>
#include "common/pmud_io.h"
#include "storage/yaml_storage.h"
#include "storage/redis_storage.h"
#include "spdlog/stopwatch.h"

using namespace std;
using namespace primordia::mud::storage;
using namespace primordia::mud::common;

namespace redis_storage = primordia::mud::storage::redis;

void usage() {
  fmt::print("Usage: redis-stream-in [-s stream-name]\n\n");
  fmt::print("NOTE:\n    REDIS_HOST and REDIS_PORT should be set in environment.\n\n");
}

int main(int argc, char** argv) {
  spdlog::set_level(spdlog::level::info);

  string stream_name;

  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "s:p:h")) != -1) {
    switch (c) {
    case 's':
      stream_name = optarg;
      break;
    case 'h':
      usage();
      return EXIT_SUCCESS;
    default:
      usage();
      return EXIT_FAILURE;
    }
  }

  if (stream_name.empty()) {
    spdlog::error("Error, stream name must be specified with -s");
    return EXIT_FAILURE;
  }

  auto storage = redis_storage::initialize_redis_storage();

  if (!storage) {
    spdlog::error("Failed to initialize redis storage.");
    return EXIT_FAILURE;
  }

  redis_storage::RedisStorage* redis_storage = dynamic_cast<redis_storage::RedisStorage*>(storage.get());
  size_t counter = 1;
  spdlog::stopwatch sw;
  while (true) {
    redis_storage->raw(fmt::format("XADD {} * sensor-id 1234 temperature {}", stream_name, counter++));
    if (counter % 1000 == 0) {
      auto elapsed = sw.elapsed();
      spdlog::info("Sent {} records in {:.3} seconds ({:.3}mps)", counter, sw, 1000 / elapsed.count());
      sw.reset();
    }
  }

  return EXIT_SUCCESS;
}