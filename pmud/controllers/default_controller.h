#pragma once

#include <string>

#include "caf/all.hpp"
#include "player/player_type_id.h"
#include "pnet/server_state.h"
#include "pnet/user_client.h"

namespace primordia::mud::player {

  using namespace caf;
  using namespace std;

  class DefaultController : public UserClient {
  public:
    DefaultController(actor_config& cfg, strong_actor_ptr command_actor)
        : UserClient(cfg, command_actor) {
    }

    behavior make_behavior() override {
      return {
        [this](OnUserInput, string input) {
          if (input == "help") {
            emit_help();
            prompt_user();
          } else {
            emit_user(fmt::format("Unknown command: {}", input));
          }
        },
      };
    }

  private:
    void emit_help() {
      emit_user("-=-=-=-=-=-=--=--==-==-=---=--=-==-==-=-=-=-===--=");
      emit_user("Primordia MUD Help\n");
      emit_user("exit or quit             quits the MUD");
      emit_user("help                     this message");
      emit_user("-=-=-=-=-=-=--=--==-==-=---=--=-==-==-=-=-=-===--=");
      emit_user("\n");
    }
  };
} // namespace primordia::mud::player
