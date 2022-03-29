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
          m_active_controller(nullptr),
          m_connection(connection),
          m_mud(mud) {

      m_default_controller = actor_cast<strong_actor_ptr>(spawn<DefaultController>(actor_cast<strong_actor_ptr>(this)));

      attach_functor([this](const error& reason) {
        spdlog::info("CommandActor({}) exiting, reason={}", this->m_connection, to_string(reason));
        if (this->m_active_controller)
          send_exit(actor_cast<actor>(this->m_active_controller), exit_reason::user_shutdown);
        if (this->m_default_controller)
          send_exit(actor_cast<actor>(this->m_default_controller), exit_reason::user_shutdown);
        this->m_active_controller.reset();
        this->m_default_controller.reset();
      });
    }

    behavior make_behavior() override {
      return {
        [this](PerformWelcome) {
          spdlog::debug("CommandActor::PerformWelcome({})", m_connection);

          auto login_controller = spawn<LoginController>(m_mud, actor_cast<strong_actor_ptr>(this));
          m_active_controller = actor_cast<strong_actor_ptr>(login_controller);
          send(login_controller, LoginControllerStart());
        },
        [this](OnUserInput, string input) {
          spdlog::debug("CommandActor::OnUserInput({}) \"{}\"", m_connection, input);
          if (m_active_controller == nullptr) {
            CommStatic comm(m_connection);
            prompt_user();
          } else {
            send(actor_cast<actor>(m_active_controller), OnUserInput(), input);
          }
        },
        [this](ToUserPrompt, string prompt) {
          spdlog::debug("CommandActor::ToUserPrompt({}) \"{}\"", m_connection, prompt);
          prompt_user(prompt);
        },
        [this](ToUserEmit, string emission) {
          spdlog::debug("CommandActor::ToUserEmit({}) \"{}\"", m_connection, emission);
          emit_user(emission);
        },
        [this](LoginControllerEnd) {
          spdlog::debug("CommandActor::LoginControllerEnd({})", m_connection);
          send_exit(actor_cast<actor>(m_active_controller), exit_reason::user_shutdown);
          m_active_controller.reset();
          m_active_controller = actor_cast<strong_actor_ptr>(m_default_controller);
          prompt_user();
        },
      };
    }

  private:
    strong_actor_ptr m_active_controller;
    int m_connection;
    strong_actor_ptr m_default_controller;
    MudSystemPtr m_mud;
  };

} // namespace primordia::mud::pnet