#pragma once

#include <string>

#include "caf/all.hpp"
#include "player/player_type_id.h"
#include "pnet/server_state.h"
#include "pnet/user_client.h"

namespace primordia::mud::player {

  using namespace caf;
  using namespace std;

  constexpr chrono::seconds LOGIN_TIMEOUT = 60s;

  const string SUPPRESS_ECHO_SEQUENCE = { (char)0xFF, (char)0xFB, (char)0x01 }; // IAC WON'T ECHO
  const string ENABLE_ECHO_SEQUENCE = { (char)0xFF, (char)0xFC, (char)0x01 };   // IAC WILL ECHO

  class LoginController : public UserClient {
  public:
    LoginController(actor_config& cfg, strong_actor_ptr command_actor)
        : UserClient(cfg, command_actor, "LoginController") {
    }

    behavior make_behavior() override {
      return {
        [this](LoginControllerStart) { prompt_user("enter username"); },
        [this](OnUserInput, string input) {
          if (state.username.empty()) {
            LOG_INFO("Got username {}", input);
            state.username = input;
            emit_user(SUPPRESS_ECHO_SEQUENCE);
            prompt_user("enter password");
          } else {
            LOG_INFO_1("Got password!");
            state.password = input;
            emit_user(ENABLE_ECHO_SEQUENCE);
            emit_user(fmt::format("Welcome {}!", state.username));
            end_controller();
          }
        },
      };
    }

  private:
    LoginState state;
  };

} // namespace primordia::mud::player
