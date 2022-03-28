#pragma once

#include <memory>
#include <map>
#include <caf/all.hpp>

#include "common/global_type_id.h"
#include "storage/storage_actor.h"

namespace primordia::mud::event {

  using namespace std;
  using namespace caf;
  using namespace primordia::mud::storage;

  class EventRecorderActor : public event_based_actor {

    inline static const string EVENT_STREAM_NAME = "events";

  public:
    EventRecorderActor(actor_config& cfg, strong_actor_ptr storage_actor)
        : event_based_actor(cfg),
          m_storage_actor(storage_actor) {
      attach_functor([this](const error& reason) { spdlog::info("EventRecorderActor exiting, reason: {}", to_string(reason)); });
    }

    behavior make_behavior() {
      return {
        [this](EventUserCreate, const string& username, const string& passphrase) {
          StreamRecordFields_t fields;
          fields["username"] = username;
          fields["passphrase"] = passphrase;
          send(actor_cast<actor>(m_storage_actor), StorageEventStore(), "EventUserCreate", fields);
        },
        [this](EventUserLogin, const string& username) {
          StreamRecordFields_t fields;
          fields["username"] = username;
          send(actor_cast<actor>(m_storage_actor), StorageEventStore(), "EventUserLogin", fields);
        },
        [this](EventUserLogout, const string& username) {
          StreamRecordFields_t fields;
          fields["username"] = username;
          send(actor_cast<actor>(m_storage_actor), StorageEventStore(), "EventUserLogout", fields);
        },
      };
    }

  private:
    strong_actor_ptr m_storage_actor;
  };

} // namespace primordia::mud::event