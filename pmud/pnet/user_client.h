#pragma once

#include "caf/all.hpp"
#include "pnet/server_state.h"

namespace primordia::mud {

  using namespace caf;

  class UserClient : public event_based_actor {
  public:
    UserClient(actor_config& cfg, strong_actor_ptr command_actor)
        : event_based_actor(cfg),
          m_command_actor(command_actor) {
    }

    void prompt_user(const string& prompt = CommStatic::DEFAULT_PROMPT) {
      send(actor_cast<actor>(m_command_actor), ToUserPrompt_v, prompt);
    }

    void emit_user(const string& emission = "") {
      send(actor_cast<actor>(m_command_actor), ToUserEmit_v, emission);
    }

    void end_controller() {
      send(actor_cast<actor>(m_command_actor), EndController_v);
    }

  private:
    strong_actor_ptr m_command_actor;
  };
} // namespace primordia::mud