#pragma once

#include <string>

#include "caf/all.hpp"
#include "player_type_id.h"
#include "pnet/server_state.h"

namespace primordia::mud::player {

  using namespace caf;
  using namespace std;

  constexpr chrono::seconds LOGIN_TIMEOUT = 60s;

  behavior LoginCoordinator(stateful_actor<LoginState>* self, int connection) {
    self->state.connection = connection;
    return {
      [=](StartLogin) {
        auto connection_actor = primordia::mud::find_connection_actor(self->system(), connection);
        // self->send(connection_actor, RegisterConnectionListener_v, self);
        self->send(connection_actor, LOGIN_TIMEOUT, WaitForInput_v, true);
      },
      [=](NotifyUserInput, string input) {
        if (self->state.username.empty()) {
          LOG_INFO("Got username {}", input);
          self->state.username = input;
        } else {
          LOG_INFO("Got password {}", input);
          self->state.password = input;
        }
      },
    };
  }

} // namespace primordia::mud::player
