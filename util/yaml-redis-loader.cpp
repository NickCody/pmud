#include "spdlog/spdlog.h"

#include "storage/storage.h"

using namespace primordia::mud::storage;

int main(int /*argc*/, char** /*argv*/) {

  auto storage = initialize_redis_storage();

  if (storage && !storage->init()) {
    SPDLOG_ERROR("Failed to initialize redis storage.");
    exit(1);
  }

  return 0;
}