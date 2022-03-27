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
      attach_functor([this](const error& reason) { SPDLOG_INFO("EventRecorderActor exiting, reason: {}", to_string(reason)); });
    }

    behavior make_behavior() {
      return {
        [this](EventUserCreate, const string& username, const string& password) {
          StreamRecordFields_t fields;
          fields["event"] = "EventUserCreate"; // TODO: use types
          fields["username"] = username;
          fields["password"] = password;
          send(actor_cast<actor>(m_storage_actor), StorageStreamStore(), EVENT_STREAM_NAME, fields);
        },
        [this](EventUserLogin, const string& username) {
          StreamRecordFields_t fields;
          fields["event"] = "EventUserLogin"; // TODO: use types
          fields["username"] = username;
          send(actor_cast<actor>(m_storage_actor), StorageStreamStore(), EVENT_STREAM_NAME, fields);
        },
        [this](EventUserLogout, const string& username) {
          StreamRecordFields_t fields;
          fields["event"] = "EventUserLogout"; // TODO: use types
          fields["username"] = username;
          send(actor_cast<actor>(m_storage_actor), StorageStreamStore(), EVENT_STREAM_NAME, fields);
        },
      };
    }

  private:
    strong_actor_ptr m_storage_actor;
  };

} // namespace primordia::mud::event