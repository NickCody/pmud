
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
using namespace primordia::mud::test::mocks;

struct basic_functionality : test_coordinator_fixture<> {
  actor storage_actor;

  basic_functionality() {
    spdlog::set_level(spdlog::level::level_enum::debug);

    unique_ptr<Storage> mock_storage = std::make_unique<MockStorage>();
    storage_actor = sys.spawn<StorageActor>(std::move(mock_storage));

    run();
  }
};

CAF_TEST_FIXTURE_SCOPE(storage_tests, basic_functionality)

CAF_TEST(basic_test) {
  self->send(storage_actor, StorageNoop_v);
  sched.run_once();
  expect((StorageNoop), from(self).to(storage_actor).with(_));

  self->send_exit(storage_actor, caf::exit_reason::user_shutdown);
}
CAF_TEST_FIXTURE_SCOPE_END()