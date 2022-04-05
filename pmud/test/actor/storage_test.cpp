
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

int main(int argc, char** argv) {
  using namespace caf;
  init_global_meta_objects<caf::id_block::pmud_caf_types>();
  core::init_global_meta_objects();
  return test::main(argc, argv);
}

struct basic_functionality : test_coordinator_fixture<> {
  actor storage_actor;

  basic_functionality() {
    spdlog::set_level(spdlog::level::level_enum::debug);

    unique_ptr<Storage> mock_storage = std::make_unique<MockStorage>();
    storage_actor = sys.spawn<StorageActor>(std::move(mock_storage));

    run();
  }

  ~basic_functionality() {
    anon_send_exit(storage_actor, exit_reason::user_shutdown);
  }
};

CAF_TEST_FIXTURE_SCOPE(storage_tests, basic_functionality)

CAF_TEST(basic_test) {
  self->send(storage_actor, StorageNoop_v);

  expect((StorageNoop), from(self).to(storage_actor).with(_));

  //  No further messages allowed.
  disallow((StorageNoop), from(self).to(storage_actor));
}
CAF_TEST_FIXTURE_SCOPE_END()