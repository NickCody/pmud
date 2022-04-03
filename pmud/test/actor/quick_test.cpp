#include <memory>
#include <iostream>
#include <caf/all.hpp>
#include <caf/test/unit_test_impl.hpp>
#include <caf/test/dsl.hpp>

CAF_BEGIN_TYPE_ID_BLOCK(demo_types, caf::first_custom_type_id)
CAF_ADD_ATOM(demo_types, pmud, Noop)
CAF_END_TYPE_ID_BLOCK(demo_types)

using namespace std;
using namespace caf;

using Message = pmud::Noop;

// using Message = caf::get_atom;

behavior TestActor(event_based_actor*) {
  return { [=](Message) {} };
}

struct basic_demo_fixture : test_coordinator_fixture<> {
  actor my_actor;

  basic_demo_fixture() {
    my_actor = sys.spawn(TestActor);
    run();
  }
};

CAF_TEST_FIXTURE_SCOPE(demo_tests, basic_demo_fixture)

CAF_TEST(quick test) {
  self->send(my_actor, Message());
  sched.run_once();

  expect((Message), from(self).to(my_actor).with(_));

  //  No further messages allowed.
  disallow((Message), from(self).to(my_actor).with(_));
}
CAF_TEST_FIXTURE_SCOPE_END()