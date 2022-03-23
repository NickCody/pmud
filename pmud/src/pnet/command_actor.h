#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/core.h>

#include "caf/stateful_actor.hpp"
#include "server_state.h"
#include "util.h"
#include "spdlog/spdlog.h"

#include "controllers/login_controller.h"
#include "controllers/default_controller.h"

#include "system/pmud_system.h"

namespace primordia::mud::pnet {

  using namespace std;
  using namespace fmt;
  using namespace caf;
  using namespace primordia::mud::system;
  using namespace primordia::mud::player;

  class CommandActor : public UserClient {
  public:
    CommandActor(actor_config& cfg, MudSystemPtr mud, strong_actor_ptr connection_actor, int connection)
        : UserClient(cfg, connection_actor, "Command"),
          m_mud(mud) {
      state.connection = connection;
      state.active_controller = nullptr;
      state.default_controller = actor_cast<strong_actor_ptr>(spawn<DefaultController>(actor_cast<strong_actor_ptr>(this)));

      attach_functor([this](const error& /*reason*/) {
        send_exit(actor_cast<actor>(state.active_controller), exit_reason::user_shutdown);
        send_exit(actor_cast<actor>(state.default_controller), exit_reason::user_shutdown);
        state.active_controller.reset();
        state.default_controller.reset();
      });
    }

    behavior make_behavior() override {
      return {
        [this](PerformWelcome) {
          auto login_controller = spawn<LoginController>(m_mud, actor_cast<strong_actor_ptr>(this));
          state.active_controller = actor_cast<strong_actor_ptr>(login_controller);
          send(login_controller, LoginControllerStart());
        },
        [this](OnUserInput, string input) {
          if (state.active_controller == nullptr) {
            CommStatic comm(state.connection);
            prompt_user();
          } else {
            send(actor_cast<actor>(state.active_controller), OnUserInput(), input);
          }
        },
        [this](ToUserPrompt, string prompt) { prompt_user(prompt); },
        [this](ToUserEmit, string emission) { emit_user(emission); },
        [this](LoginControllerEnd) {
          send_exit(actor_cast<actor>(state.active_controller), exit_reason::user_shutdown);
          state.active_controller.reset();
          state.active_controller = actor_cast<strong_actor_ptr>(state.default_controller);
          prompt_user();
        },
      };
    }

  private:
    CommandState state;
    strong_actor_ptr default_controller = nullptr;
    MudSystemPtr m_mud;
  };

} // namespace primordia::mud::pnet