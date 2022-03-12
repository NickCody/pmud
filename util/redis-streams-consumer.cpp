#include <chrono>

#include "spdlog/spdlog.h"
#include "common/pmud_io.h"
#include "common/yaml_storage.h"
#include "storage/storage.h"

using namespace std;
using namespace primordia::mud::storage;
using namespace primordia::mud::common;

void usage() {
  fmt::print("Usage: redis-streams-consumer [-s stream-name]\n\n");
  fmt::print("NOTE:\n    REDIS_HOST and REDIS_PORT should be set in environment.\n\n");
}

int main(int argc, char** argv) {
  spdlog::set_level(spdlog::level::debug);

  string stream_name;
  string position = "$";

  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "s:p:h")) != -1) {
    switch (c) {
    case 's':
      stream_name = optarg;
      break;
    case 'p':
      position = optarg;
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

  while (true) {
    vector<StreamResponse> streams = storage->read_stream_block(stream_name, position);
    for (auto response : streams) {
      fmt::print("stream: {}\n", response.stream_name);
      for (auto record : response.records) {
        for (auto tup : record.fields) {
          fmt::print("  {}:{}\n", tup.first, tup.second);
        }
      }
    }
    this_thread::sleep_for(std::chrono::seconds(1));
  }

  return EXIT_SUCCESS;
}