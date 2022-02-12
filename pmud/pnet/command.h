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

  behavior Command(stateful_actor<CommandState>* self, strong_actor_ptr connection_actor, int connection) {
    self->state.connection = connection;
    self->state.connection_actor = connection_actor;
    self->state.active_controller = nullptr;

    return {
      [=](UserInput, string input) {
        LOG_INFO("Received user input for connection {}: {}", self->state.connection, input);
        if (self->state.active_controller == nullptr) {
          CommStatic comm(self->state.connection);
          self->send(actor_cast<actor>(self->state.connection_actor), Prompt_v, CommStatic::DEFAULT_PROMPT);
          self->send(actor_cast<actor>(self->state.connection_actor), Prompt_v, CommStatic::DEFAULT_PROMPT);
        } else {
          self->send(actor_cast<actor>(self->state.active_controller), UserInput_v, input);
        }
      },
      [=](Prompt, string prompt) { self->send(actor_cast<actor>(self->state.connection_actor), Prompt_v, prompt); },
      [=](Emit, string emission) { self->send(actor_cast<actor>(self->state.connection_actor), Emit_v, emission); },
      [=](Welcome) {
        auto login_controller = self->spawn(LoginController, actor_cast<strong_actor_ptr>(self));
        self->state.active_controller = actor_cast<strong_actor_ptr>(login_controller);
        self->send(login_controller, StartLogin_v);
      },
      [=](EndLogin) {
        self->state.active_controller = nullptr;
        self->send(actor_cast<actor>(self->state.connection_actor), Prompt_v, CommStatic::DEFAULT_PROMPT);
      },
    };
  }

} // namespace primordia::mud