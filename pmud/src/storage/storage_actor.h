#pragma once

#include <memory>
#include "storage/storage.h"
#include "system/pmud_system.h"
#include "caf/type_id.hpp"
#include "common/global_type_id.h"

CAF_BEGIN_TYPE_ID_BLOCK(storage_caf_types, primordia::mud::type_id::first_storage_type_id)

CAF_ADD_ATOM(storage_caf_types, StorageValueStore)
CAF_ADD_ATOM(storage_caf_types, StorageMapStore)

CAF_END_TYPE_ID_BLOCK(storage_caf_types)

namespace primordia::mud::storage {

  using namespace std;

  class StorageActor : public event_based_actor {
  public:
    StorageActor(actor_config& cfg, unique_ptr<Storage> storage)
        : event_based_actor(cfg),
          m_storage(move(storage)) {}

    behavior make_behavior() {
      return {
        [this](StorageValueStore, const string& key, const string& value) -> bool { return m_storage->value_store(key, value); },
        [this](StorageMapStore, const string& map_name, const string& key, const string& value) -> bool {
          return m_storage->map_store(map_name, key, value);
        },
      };
    }

  private:
    unique_ptr<Storage> m_storage;
  };

} // namespace primordia::mud::storage