#pragma once

#include <memory>
#include <optional>
#include <caf/all.hpp>
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
            m_storage->value_store(key, value);
          }
        },
        [this](StorageValueGet, const string& key) -> string {
          auto val = m_storage->value_get(key);

          if (m_storage && val.has_value()) {
            return m_storage->value_get(key).value();
          }
          return "";
        },
        [this](StorageMapStore, const string& map_name, const string& key, const string& value) {
          if (m_storage) {
            m_storage->map_store(map_name, key, value);
          }
        },
        [this](StorageListStore, const string& list_name, const string& value) {
          if (m_storage) {
            m_storage->list_store(list_name, value);
          }
        },
        [this](StorageSetStore, const string& set_name, const string& value) {
          if (m_storage) {
            m_storage->set_store(set_name, value);
          }
        },
        [this](StorageStreamStore, const string& stream_name, const StreamRecordFields_t& fields) {
          if (m_storage) {
            m_storage->stream_store(stream_name, fields);
          }
        },
        [this](StorageEventStore, const string& event_name, const StreamRecordFields_t& fields) {
          if (m_storage) {
            m_storage->event_store(event_name, fields);
          }
        },
      }; // namespace primordia::mud::storage
    }

  private:
    unique_ptr<Storage> m_storage;
  };

} // namespace primordia::mud::storage