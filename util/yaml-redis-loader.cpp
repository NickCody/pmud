#include "spdlog/spdlog.h"
#include "common/io.h"
#include "common/yaml_util.h"
#include "storage/storage.h"

using namespace std;
using namespace primordia::mud::storage;
using namespace primordia::mud::common;

void usage() {
  fmt::print("Usage: yaml-redis-loader [-f filename] [-p prefix]\n\n");
  fmt::print("NOTE:\n    REDIS_HOST and REDIS_PORT should be set in environment.\n\n");
}

int main(int argc, char** argv) {

  string filename;
  string prefix;

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

  auto storage = initialize_redis_storage();

  if (!storage) {
    SPDLOG_ERROR("Failed to initialize redis storage.");
    return EXIT_FAILURE;
  }

  auto input = stdout_or_file(filename.c_str());

  YAML::Node config = YAML::Load(*input);

  auto buffer = fmt::memory_buffer();
  node_to_buffer(buffer, config);
  fmt::print("{}", buffer.data());
  return EXIT_SUCCESS;
}