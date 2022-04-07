
#include "pmud/src/common/global_type_id.h"
#include <algorithm>
#include <caf/scoped_actor.hpp>
#include <chrono>
#include <memory>
#include <fmt/format.h>

#include <caf/all.hpp>
#define CAF_TEST_NO_MAIN
#include <caf/test/unit_test_impl.hpp>
#include <caf/test/dsl.hpp>

#include "storage/storage.h"
#include "storage/storage_actor.h"
#include "pmud/test/mocks/mock_storage.h"

#include <spdlog/spdlog.h>
#include "common/global_type_id.h"

using namespace std;
using namespace caf;

using namespace primordia::mud::storage;
using namespace primordia::mud::test::mocks;

namespace common = primordia::mud::common;

const auto TEST_TIMEOUT = chrono::milliseconds(20);

int main(int argc, char** argv) {
  using namespace caf;
  init_global_meta_objects<caf::id_block::pmud_caf_types>();
  core::init_global_meta_objects();
  return test::main(argc, argv);
}

struct basic_functionality {
  caf::actor_system_config cfg;
  caf::actor_system sys;
  caf::scoped_actor self;
  actor storage_actor;

  basic_functionality()
      : sys(cfg),
        self(sys) {
    spdlog::set_level(spdlog::level::level_enum::debug);

    unique_ptr<Storage> mock_storage = std::make_unique<MockStorage>();
    storage_actor = sys.spawn<StorageActor>(std::move(mock_storage));
  }

  ~basic_functionality() {
    anon_send_exit(storage_actor, exit_reason::user_shutdown);
  }
};

CAF_TEST_FIXTURE_SCOPE(storage_tests, basic_functionality)

CAF_TEST(value store) {
  self->send(storage_actor, StorageValueStore(), "foo", "bar");
  self->request(storage_actor, TEST_TIMEOUT, StorageValueGet(), "foo")
      .receive([](const common::opt_string_t& value) { CAF_CHECK(value.value() == "bar"); }, [](error&) { CAF_CHECK(false); });
}

CAF_TEST(map store) {
  self->send(storage_actor, StorageMapStore(), "mymap", "foo", "bar");
  self->request(storage_actor, TEST_TIMEOUT, StorageMapGet(), "mymap")
      .receive([](const common::opt_map_t& value) { CAF_CHECK(value.value().at("foo") == "bar"); }, [](error&) { CAF_CHECK(false); });
}

CAF_TEST(list store) {
  self->send(storage_actor, StorageListStore(), "mylist", "foo");
  self->send(storage_actor, StorageListStore(), "mylist", "bar");
  self->request(storage_actor, TEST_TIMEOUT, StorageListGet(), "mylist")
      .receive(
          [](const common::opt_list_t& value) {
            CAF_CHECK(std::find(value.value().begin(), value.value().end(), "foo") != value.value().end());
            CAF_CHECK(std::find(value.value().begin(), value.value().end(), "bar") != value.value().end());
          },
          [](error&) { CAF_CHECK(false); });
}

CAF_TEST(set store) {
  self->send(storage_actor, StorageSetStore(), "myset", "foo");
  self->send(storage_actor, StorageSetStore(), "myset", "bar");
  self->request(storage_actor, TEST_TIMEOUT, StorageSetGet(), "myset")
      .receive(
          [](const common::opt_set_t& value) {
            CAF_CHECK(std::find(value.value().begin(), value.value().end(), "foo") != value.value().end());
            CAF_CHECK(std::find(value.value().begin(), value.value().end(), "bar") != value.value().end());
          },
          [](error&) { CAF_CHECK(false); });
}

CAF_TEST_FIXTURE_SCOPE_END()