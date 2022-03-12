#pragma once

#include <string>

#include "caf/type_id.hpp"
#include "common/global_type_id.h"

namespace primordia::mud::player {
  using namespace std;

  struct LoginState {
    string username;
    string password;
  };
} // namespace primordia::mud::player

CAF_BEGIN_TYPE_ID_BLOCK(player_caf_types, primordia::mud::type_id::first_player_type_id)

CAF_ADD_TYPE_ID(player_caf_types, (primordia::mud::player::LoginState))

CAF_ADD_ATOM(player_caf_types, LoginControllerStart)
CAF_ADD_ATOM(player_caf_types, LoginControllerEnd)

CAF_END_TYPE_ID_BLOCK(player_caf_types)

namespace primordia::mud::player {
  template <class Inspector> bool inspect(Inspector& f, LoginState& x) {
    return f.object(x).fields(f.field("username", x.username), f.field("password", x.password));
  }
} // namespace primordia::mud::player