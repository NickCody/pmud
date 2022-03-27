#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <caf/stateful_actor.hpp>

#include "system/pmud_system.h"
#include "util.h"
#include "controllers/login_controller.h"
#include "controllers/default_controller.h"

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
      connection = connection;
      active_controller = nullptr;
      default_controller = actor_cast<strong_actor_ptr>(spawn<DefaultController>(actor_cast<strong_actor_ptr>(this)));

      attach_functor([this](const error& reason) {
        spdlog::info("CommandActor({}) exiting, reason={}", this->connection, to_string(reason));
        if (active_controller)
          send_exit(actor_cast<actor>(active_controller), exit_reason::user_shutdown);
        if (default_controller)
          send_exit(actor_cast<actor>(default_controller), exit_reason::user_shutdown);
        active_controller.reset();
        default_controller.reset();
      });
    }

    behavior make_behavior() override {
      return {
        [this](PerformWelcome) {
          spdlog::debug("CommandActor::PerformWelcome({})", connection);

          auto login_controller = spawn<LoginController>(m_mud, actor_cast<strong_actor_ptr>(this));
          active_controller = actor_cast<strong_actor_ptr>(login_controller);
          send(login_controller, LoginControllerStart());
        },
        [this](OnUserInput, string input) {
          spdlog::debug("CommandActor::PerformWelcome({}) \"{}\"", connection, input);
          if (active_controller == nullptr) {
            CommStatic comm(connection);
            prompt_user();
          } else {
            send(actor_cast<actor>(active_controller), OnUserInput(), input);
          }
        },
        [this](ToUserPrompt, string prompt) {
          spdlog::debug("CommandActor::ToUserPrompt({}) \"{}\"", connection, prompt);
          prompt_user(prompt);
        },
        [this](ToUserEmit, string emission) {
          spdlog::debug("CommandActor::ToUserEmit({}) \"{}\"", connection, emission);
          emit_user(emission);
        },
        [this](LoginControllerEnd) {
          spdlog::debug("CommandActor::LoginControllerEnd({})", connection);
          send_exit(actor_cast<actor>(active_controller), exit_reason::user_shutdown);
          active_controller.reset();
          active_controller = actor_cast<strong_actor_ptr>(default_controller);
          prompt_user();
        },
      };
    }

  private:
    int connection;
    strong_actor_ptr active_controller;
    strong_actor_ptr default_controller;
    MudSystemPtr m_mud;
  };

} // namespace primordia::mud::pnet