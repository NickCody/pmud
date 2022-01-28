#include <iostream>

#include "caf/all.hpp"
#include "caf/actor_system.hpp"
#include "caf/caf_main.hpp"
#include "caf/event_based_actor.hpp"

using namespace caf;
using namespace std;

struct Foo;
struct Bar;

CAF_BEGIN_TYPE_ID_BLOCK(custom_types_1, first_custom_type_id)
CAF_ADD_TYPE_ID(custom_types_1, (Foo))
CAF_ADD_TYPE_ID(custom_types_1, (Bar))
CAF_END_TYPE_ID_BLOCK(custom_types_1)

struct Foo {
  int bar;
};

template <class Inspector> bool inspect(Inspector& f, Foo& x) {
  return f.object(x).fields(f.field("bar", x.bar));
}

struct Bar {
  int foo;
};

template <class Inspector> bool inspect(Inspector& f, Bar& x) {
  return f.object(x).fields(f.field("foo", x.foo));
}

behavior posh(event_based_actor* self) {
  return {
    [=](const Foo& val) { aout(self) << "Foo = " << deep_to_string(val) << endl; },
    [=](const Bar& val) { aout(self) << "Bar = " << deep_to_string(val) << endl; },
  };
}

void caf_main(actor_system& sys) {
  Foo f{ 10 };
  Bar b{ 20 };
  auto posh_actor = sys.spawn(posh);
  scoped_actor self{ sys };
  self->send(posh_actor, f);
  self->send(posh_actor, b);
}

CAF_MAIN(id_block::custom_types_1)