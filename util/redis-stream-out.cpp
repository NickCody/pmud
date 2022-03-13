#include <chrono>
#include <stdlib.h>

#include "spdlog/spdlog.h"
#include "spdlog/async.h" //support for async logging.
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "common/pmud_io.h"
#include "common/yaml_storage.h"
#include "storage/redis_storage.h"
#include "spdlog/stopwatch.h"

using namespace std;
using namespace primordia::mud::storage;
using namespace primordia::mud::common;

namespace redis_storage = primordia::mud::storage::redis;

void usage() {
  fmt::print("Usage: redis-stream-out [-s stream-name]\n\n");
  fmt::print("NOTE:\n    REDIS_HOST and REDIS_PORT should be set in environment.\n\n");
}

const size_t DEFAULT_BATCH_SIZE = 10000;

int main(int argc, char** argv) {
  // auto console = spdlog::stdout_color_mt("console");
  // spdlog::set_default_logger(console);
  spdlog::set_level(spdlog::level::info);

  string stream_name;
  size_t batch_size = DEFAULT_BATCH_SIZE;
  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "s:b:h")) != -1) {
    switch (c) {
    case 's':
      stream_name = optarg;
      break;
    case 'b':
      batch_size = atoi(optarg);
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
    SPDLOG_ERROR("Error, stream name must be specified with -s");
    return EXIT_FAILURE;
  }

  auto storage = redis_storage::initialize_redis_storage();

  if (!storage) {
    SPDLOG_ERROR("Failed to initialize redis storage.");
    return EXIT_FAILURE;
  }

  size_t counter = 0;
  string position = "0";
  spdlog::stopwatch sw;
  while (true) {
    StreamResponses_t stream_responses =
        storage->read_stream_raw(fmt::format("XREAD BLOCK 0 COUNT {} STREAMS {} {}", batch_size, stream_name, position));

    for (auto response : stream_responses) {
      for (auto record : response.records) {
        counter++;
        position = record.timestamp;
        // for (auto tup : record.fields) {
        // }
        if (counter % batch_size == 0) {
          auto elapsed = sw.elapsed();
          SPDLOG_INFO("{} Read latest batch of records until {} in {:.3} seconds ({:.3f}mps) : {}",
                      record.timestamp,
                      counter,
                      sw,
                      batch_size / elapsed.count(),
                      record.fields["temperature"]);
          sw.reset();
        }
      }
    }
  }

  return EXIT_SUCCESS;
}