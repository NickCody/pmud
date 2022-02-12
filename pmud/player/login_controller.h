#pragma once

#include <string>

#include "caf/all.hpp"
#include "player_type_id.h"
#include "pnet/server_state.h"

namespace primordia::mud::player {

  using namespace caf;
  using namespace std;

  constexpr chrono::seconds LOGIN_TIMEOUT = 60s;

  const string SUPPRESS_ECHO_SEQUENCE = { (char)0xFF, (char)0xFB, (char)0x01 }; // IAC WON'T ECHO
  const string ENABLE_ECHO_SEQUENCE = { (char)0xFF, (char)0xFC, (char)0x01 };   // IAC WILL ECHO

  behavior LoginController(stateful_actor<LoginState>* self, strong_actor_ptr command_actor) {
    self->state.command_actor = command_actor;

    return {
      [=](LoginControllerStart) { self->send(actor_cast<actor>(self->state.command_actor), ToUserPrompt_v, "enter username"); },
      [=](OnUserInput, string input) {
        if (self->state.username.empty()) {
          LOG_INFO("Got username {}", input);
          self->state.username = input;
          self->send(actor_cast<actor>(self->state.command_actor), ToUserEmit_v, SUPPRESS_ECHO_SEQUENCE);
          self->send(actor_cast<actor>(self->state.command_actor), ToUserPrompt_v, "enter password");
        } else {
          LOG_INFO_1("Got password!");
          self->state.password = input;
          self->send(actor_cast<actor>(self->state.command_actor), ToUserEmit_v, ENABLE_ECHO_SEQUENCE);
          self->send(actor_cast<actor>(self->state.command_actor), ToUserEmit_v, fmt::format("Welcome {}!", self->state.username));
          self->send(actor_cast<actor>(self->state.command_actor), EndController_v);
        }
      },
    };
  }

} // namespace primordia::mud::player
