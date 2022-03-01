#include "common/io.h"

#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"

#include "storage/storage.h"

using namespace std;
using namespace primordia::mud::storage;
using namespace primordia::mud::common;

int main(int argc, char** argv) {

  auto storage = initialize_redis_storage();

  if (!storage) {
    SPDLOG_ERROR("Failed to initialize redis storage.");
    exit(1);
  }

  string filename;
  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "f:")) != -1) {
    switch (c) {
    case 'f':
      filename = optarg;
      break;
    default:
      exit(1);
    }
  }

  auto input = stdout_or_file(filename.c_str());

  YAML::Node config = YAML::Load(*input);
  fmt::print("{}\n", config["server"]["name"].as<string>());
  return 0;
}