#pragma once

#include <memory>
#include "storage/storage.h"
#include "system/pmud_system.h"
#include "caf/type_id.hpp"
#include "common/global_type_id.h"

namespace primordia::mud::storage {

  using namespace std;
  using namespace caf;

  class StorageActor : public event_based_actor {
  public:
    StorageActor(actor_config& cfg, unique_ptr<Storage> storage)
        : event_based_actor(cfg),
          m_storage(move(storage)) {

      attach_functor([this](const error& reason) { SPDLOG_INFO("StorageActor exiting, reason: {}", to_string(reason)); });
    }

    behavior make_behavior() {
      return {
        [this](StorageValueStore, const string& key, const string& value) { m_storage->value_store(key, value); },
        [this](StorageMapStore, const string& map_name, const string& key, const string& value) { m_storage->map_store(map_name, key, value); },
        [this](StorageStreamStore, const string& stream_name, const StreamRecordFields_t& fields) { m_storage->stream_store(stream_name, fields); },
      };
    }

  private:
    unique_ptr<Storage> m_storage;
  };

} // namespace primordia::mud::storage