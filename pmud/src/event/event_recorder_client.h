#pragma once

#include <string>
#include <caf/all.hpp>
#include "common/global_type_id.h"

namespace primordia::mud::event {

  using namespace std;
  using namespace caf;

  class EventRecorderClient {
  public:
    explicit EventRecorderClient(strong_actor_ptr event_actor)
        : m_event_actor(event_actor) {}

    void record_event_user_create(const string& username, const string& passphrase) {
      caf::anon_send(actor_cast<actor>(m_event_actor), EventUserCreate(), username, passphrase);
    }

    void record_event_user_login(const string& username) {
      caf::anon_send(actor_cast<actor>(m_event_actor), EventUserLogin(), username);
    }

    void record_event_user_logout(const string& username) {
      caf::anon_send(actor_cast<actor>(m_event_actor), EventUserLogout(), username);
    }

  private:
    strong_actor_ptr m_event_actor;
  };
} // namespace primordia::mud::event