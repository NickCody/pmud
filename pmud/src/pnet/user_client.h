#pragma once

#include "caf/all.hpp"
#include "pnet/server_state.h"

namespace primordia::mud::pnet {

  using namespace caf;

  class UserClient : public event_based_actor {
  public:
    UserClient(actor_config& cfg, strong_actor_ptr connection_actor, const string& name)
        : event_based_actor(cfg),
          m_connection_actor(connection_actor),
          m_name(name) {}

    virtual ~UserClient() {
      m_connection_actor.reset();
    }

    void prompt_user(const string& prompt = CommStatic::DEFAULT_PROMPT) {
      send(actor_cast<actor>(m_connection_actor), ToUserPrompt(), prompt);
    }

    void emit_user(const string& emission = "") {
      send(actor_cast<actor>(m_connection_actor), ToUserEmit(), emission);
    }

    void end_controller() {
      send(actor_cast<actor>(m_connection_actor), LoginControllerEnd());
    }

    void funky_banner() {
      emit_user("-=-=-=-=-=-=--=--==-==-=---=--=-==-==-=-=-=-===--=");
    }

  private:
    strong_actor_ptr m_connection_actor;
    string m_name;
  };
} // namespace primordia::mud::pnet