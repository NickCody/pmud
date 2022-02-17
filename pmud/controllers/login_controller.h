#pragma once

#include <string>

#include "caf/all.hpp"
#include "player/player_type_id.h"
#include "pnet/server_state.h"
#include "pnet/user_client.h"
#include "system/pmud_system.h"

namespace primordia::mud::player {

  using namespace caf;
  using namespace std;
  using namespace system;

  constexpr chrono::seconds LOGIN_TIMEOUT = 60s;

  const string SUPPRESS_ECHO_SEQUENCE = { (char)0xFF, (char)0xFB, (char)0x01 }; // IAC WON'T ECHO
  const string ENABLE_ECHO_SEQUENCE = { (char)0xFF, (char)0xFC, (char)0x01 };   // IAC WILL ECHO

  class LoginController : public UserClient {
  public:
    LoginController(actor_config& cfg, MudSystemPtr mud, strong_actor_ptr command_actor)
        : UserClient(cfg, command_actor, "LoginController"),
          m_mud(mud) {
    }

    behavior make_behavior() override {
      return {
        [this](LoginControllerStart) { prompt_user("enter username"); },
        [this](OnUserInput, string input) {
          if (state.username.empty()) {
            state.username = input;
            emit_user(SUPPRESS_ECHO_SEQUENCE);
            prompt_user("enter password");
          } else if (state.password.empty()) {
            state.password = input;
            prompt_user("confirm password");
          } else {
            if (state.password == input) {
              emit_user();
              emit_user(fmt::format("Welcome to Primordia {}!", state.username));
              emit_user();
              end_controller();
            } else {
              emit_user();
              emit_user(fmt::format("Passwords don't match, please try again.", state.username));
              emit_user(SUPPRESS_ECHO_SEQUENCE);
              prompt_user("enter password");
            }
          }
        },
      };
    }

  private:
    LoginState state;
    MudSystemPtr m_mud;
  };

} // namespace primordia::mud::player
