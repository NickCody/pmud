#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/format.h>

#include "caf/stateful_actor.hpp"
#include "server_state.h"
#include "util.h"
#include "logger/logger.h"

#include "player/login_controller.h"

namespace primordia::mud {

  using namespace fmt;
  using namespace caf;
  using namespace std;

  using namespace player;

  class Command : public event_based_actor {
  public:
    Command(actor_config& cfg, strong_actor_ptr connection_actor, int connection)
        : event_based_actor(cfg) {
      state.connection = connection;
      state.connection_actor = connection_actor;
      state.active_controller = nullptr;
    }

    behavior make_behavior() override {
      return {
        [this](PerformWelcome) {
          auto login_controller = spawn(LoginController, actor_cast<strong_actor_ptr>(this));
          state.active_controller = actor_cast<strong_actor_ptr>(login_controller);
          send(login_controller, LoginControllerStart_v);
        },
        [this](OnUserInput, string input) {
          LOG_INFO("Received user input for connection {}: {}", state.connection, input);
          if (state.active_controller == nullptr) {
            CommStatic comm(state.connection);
            prompt_user();
          } else {
            send(actor_cast<actor>(state.active_controller), OnUserInput_v, input);
          }
        },
        [this](ToUserPrompt, string prompt) { prompt_user(prompt); },
        [this](ToUserEmit, string emission) { emit_user(emission); },
        [this](EndController) {
          state.active_controller = nullptr;
          prompt_user();
        },
      };
    }

    void prompt_user(const string& prompt = CommStatic::DEFAULT_PROMPT) {
      send(actor_cast<actor>(state.connection_actor), ToUserPrompt_v, prompt);
    }

    void emit_user(const string& emission = "") {
      send(actor_cast<actor>(state.connection_actor), ToUserEmit_v, emission);
    }

  private:
    CommandState state;
  };

} // namespace primordia::mud