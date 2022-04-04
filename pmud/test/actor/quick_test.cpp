#include <caf/fwd.hpp>
#include <memory>
#include <iostream>
#include <caf/all.hpp>
#define CAF_TEST_NO_MAIN
#include <caf/test/unit_test_impl.hpp>
#include <caf/test/dsl.hpp>
#include <fmt/format.h>

using namespace std;
using namespace caf;

CAF_BEGIN_TYPE_ID_BLOCK(demo_types, caf::first_custom_type_id)
CAF_ADD_ATOM(demo_types, pmud, Noop)
CAF_END_TYPE_ID_BLOCK(demo_types)

int main(int argc, char** argv) {
  using namespace caf;
  init_global_meta_objects<id_block::demo_types>();
  core::init_global_meta_objects();
  return test::main(argc, argv);
}

behavior TestActor(event_based_actor*) {
  return { [=](pmud::Noop) {} };
}

struct basic_demo_fixture : test_coordinator_fixture<> {
  actor my_actor;

  basic_demo_fixture() {
    my_actor = sys.spawn(TestActor);
    run();
  }

  ~basic_demo_fixture() {
    anon_send_exit(my_actor, exit_reason::user_shutdown);
  }
};

CAF_TEST_FIXTURE_SCOPE(demo_tests, basic_demo_fixture)

CAF_TEST(quick test) {
  self->send(my_actor, pmud::Noop());

  expect((pmud::Noop), from(self).to(my_actor));

  //  No further messages allowed.
  disallow((pmud::Noop), from(self).to(my_actor));
}
CAF_TEST_FIXTURE_SCOPE_END()