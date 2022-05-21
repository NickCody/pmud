#include <spdlog/spdlog.h>
#include "common/pmud_io.h"
#include "storage/yaml_storage.h"
#include "storage/redis_storage.h"

using namespace std;
using namespace primordia::mud::storage;
using namespace primordia::mud::common;

namespace redis_storage = primordia::mud::storage::redis;

void usage() {
  fmt::print("Usage: yaml-redis-loader [-f filename] [-p prefix]\n\n");
  fmt::print("NOTE:\n    REDIS_HOST and REDIS_PORT should be set in environment.\n\n");
}

int main(int argc, char** argv) {
  spdlog::set_level(spdlog::level::debug);

  spdlog::debug("Starting!");

  string filename;
  string prefix = "pmud";

  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "f:p:h")) != -1) {
    switch (c) {
    case 'f':
      filename = optarg;
      break;
    case 'p':
      prefix = optarg;
      break;
    case 'h':
      usage();
      return EXIT_SUCCESS;
    default:
      usage();
      return EXIT_FAILURE;
    }
  }

  if (auto storage = redis_storage::initialize_redis_storage(); storage) {
    auto input = stdout_or_file(filename.c_str());
    if (!input) {
      spdlog::info("Failed to open file: {}", filename);
      return EXIT_FAILURE;
    }

    YAML::Node config = YAML::Load(*input);
    yaml_to_storage(storage.get(), prefix, config);
  } else {
    spdlog::error("Failed to initialize redis storage.");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}