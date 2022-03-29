#pragma once

#include <string>

#include "caf/all.hpp"
#include "pnet/server_state.h"
#include "pnet/user_client.h"

namespace primordia::mud::player {

  using namespace caf;
  using namespace std;

  class DefaultController : public UserClient {
  public:
    DefaultController(actor_config& cfg, strong_actor_ptr command_actor)
        : UserClient(cfg, command_actor, "DefaultController") {}

    behavior make_behavior() override {
      return {
        [this](OnUserInput, string input) {
          if (input == "help") {
            emit_help();
            prompt_user();
          } else if (!input.empty()) {
            emit_user(fmt::format("Unknown command: {}", input));
            prompt_user();
          } else {
            prompt_user();
          }
        },
      };
    }

  private:
    void emit_help() {
      funky_banner();
      emit_user("Primordia MUD Help\n");
      emit_user("exit or quit             quits the MUD");
      emit_user("help                     this message");
      funky_banner();
      emit_user("\n");
    }
  };
} // namespace primordia::mud::player
