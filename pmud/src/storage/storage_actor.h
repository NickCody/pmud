#pragma once

#include <caf/all.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "common/global_type_id.h"
#include "storage/storage.h"
#include "system/pmud_system.h"

namespace primordia::mud::storage {

  using namespace std;
  using namespace caf;

  class StorageActor : public event_based_actor {
  public:
    StorageActor(actor_config& cfg, unique_ptr<Storage> storage)
        : event_based_actor(cfg),
          m_storage(std::move(storage)) {

      // attach_functor([](const error& reason) { spdlog::info("StorageActor exiting, reason: {}", to_string(reason)); });
    }

    behavior make_behavior() {
      return {
        [](StorageNoop) { spdlog::debug("StorageActor::Noop"); },
        [this](StorageValueStore, const string& key, const string& value) {
          if (m_storage) {
            spdlog::debug("StorageActor::StorageValueStore: {}={}", key, value);
            m_storage->value_store(key, value);
          }
        },
        [this](StorageMapStore, const string& map_name, const string& key, const string& value) {
          if (m_storage) {
            spdlog::debug("StorageActor::StorageMapStore: map={}, key={}, value={}", map_name, key, value);
            m_storage->map_store(map_name, key, value);
          }
        },
        [this](StorageStreamStore, const string& stream_name, const StreamRecordFields_t& fields) {
          if (m_storage) {

            spdlog::debug("StorageActor::StorageStreamStore: stream_name={}, size={}", stream_name, fields.size());
            m_storage->stream_store(stream_name, fields);
          }
        },
        [this](StorageEventStore, const string& event_name, const StreamRecordFields_t& fields) {
          if (m_storage) {

            spdlog::debug("StorageActor::StorageEventStore: event_name={}, size={}", event_name, fields.size());
            m_storage->event_store(event_name, fields);
          }
        },
      }; // namespace primordia::mud::storage
    }

  private:
    unique_ptr<Storage> m_storage;
  };

} // namespace primordia::mud::storage