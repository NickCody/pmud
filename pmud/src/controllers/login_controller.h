#pragma once

#include <string>

#include "caf/all.hpp"
#include "common/global_type_id.h"
#include "pnet/server_state.h"
#include "pnet/user_client.h"
#include "system/pmud_system.h"
#include "event/event_recorder_client.h"

namespace primordia::mud::player {

  using namespace caf;
  using namespace std;
  using namespace system;
  using namespace primordia::mud::event;

  constexpr chrono::seconds LOGIN_TIMEOUT = 60s;

  const string SUPPRESS_ECHO_SEQUENCE = { (char)0xFF, (char)0xFB, (char)0x01 }; // IAC WON'T ECHO
  const string ENABLE_ECHO_SEQUENCE = { (char)0xFF, (char)0xFC, (char)0x01 };   // IAC WILL ECHO

  class LoginController : public UserClient, public EventRecorderClient {
  public:
    LoginController(actor_config& cfg, MudSystemPtr mud, strong_actor_ptr command_actor)
        : UserClient(cfg, command_actor, "LoginController"),
          EventRecorderClient(mud->get_event_recorder_actor()),
          m_mud(mud) {}

    behavior make_behavior() override {
      return {
        [this](LoginControllerStart) { prompt_user("enter username"); },
        [this](OnUserInput, string input) {
          if (username.empty()) {
            username = input;
            emit_user(SUPPRESS_ECHO_SEQUENCE);
            prompt_user("enter password");
          } else if (password.empty()) {
            password = input;
            prompt_user("confirm password");
          } else {
            if (password == input) {
              emit_user();
              emit_user(fmt::format("Welcome to Primordia {}!", username));
              emit_user();
              record_event_user_create(username, password);
              end_controller();
            } else {
              emit_user();
              emit_user(fmt::format("Passwords don't match, please try again.", username));
              emit_user(SUPPRESS_ECHO_SEQUENCE);
              prompt_user("enter password");
            }
          }
        },
      };
    }

  private:
    string username;
    string password;
    MudSystemPtr m_mud;
  };

} // namespace primordia::mud::player
