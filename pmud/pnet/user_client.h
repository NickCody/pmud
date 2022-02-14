#pragma once

#include "caf/all.hpp"
#include "pnet/server_state.h"

namespace primordia::mud {

  using namespace caf;

  class UserClient : public event_based_actor {
  public:
    UserClient(actor_config& cfg, strong_actor_ptr user_client_actor, const string& name)
        : event_based_actor(cfg),
          m_user_client_actor(user_client_actor),
          m_name(name) {
    }

    virtual ~UserClient() {
      m_user_client_actor.reset();
    }

    void prompt_user(const string& prompt = CommStatic::DEFAULT_PROMPT) {
      send(actor_cast<actor>(m_user_client_actor), ToUserPrompt_v, prompt);
    }

    void emit_user(const string& emission = "") {
      send(actor_cast<actor>(m_user_client_actor), ToUserEmit_v, emission);
    }

    void end_controller() {
      send(actor_cast<actor>(m_user_client_actor), LoginControllerEnd_v);
    }

  private:
    strong_actor_ptr m_user_client_actor;
    string m_name;
  };
} // namespace primordia::mud