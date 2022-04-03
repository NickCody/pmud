
#include <memory>
#include <fmt/format.h>

#include <caf/all.hpp>
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
using namespace primordia::mud::test::mock;

struct basic_functionality : test_coordinator_fixture<> {
  actor storage_actor;

  basic_functionality() {
    spdlog::set_level(spdlog::level::level_enum::debug);
    // unique_ptr<Storage> mock_storage = std::make_unique<MockStorage>();
    storage_actor = sys.spawn<StorageActor>(nullptr);
    // storage_actor = sys.spawn<StorageActor>(nullptr);
    run();
  }
};

CAF_TEST_FIXTURE_SCOPE(storage_tests, basic_functionality)

CAF_TEST(storage basic_functionality) {
  string foo("foo");
  string bar("bar");
  self->send(storage_actor, StorageValueStore(), foo, bar);
  // sched.run_once();
  // expect((StorageValueStore, string, string), from(self).to(storage_actor).with(_, "foo", "bar"));

  self->send_exit(storage_actor, caf::exit_reason::user_shutdown);

  // No further messages allowed.
  // disallow((StorageValueStore, string, string), from(self).to(storage_actor).with(_, "", ""));
}
CAF_TEST_FIXTURE_SCOPE_END()