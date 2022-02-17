#pragma once

#include <caf/actor_system.hpp>
#include "storage/storage.h"

namespace primordia::mud::system {

  using namespace caf;
  using namespace storage;
  using namespace std;

  class MudSystem {
  public:
    MudSystem(StoragePtr storage, actor_system& caf_system)
        : m_storage(storage),
          m_actor_system(caf_system) {
    }

    StoragePtr storage() {
      return m_storage;
    }

    actor_system& actorsystem() {
      return m_actor_system;
    }

  private:
    StoragePtr m_storage;
    actor_system& m_actor_system;
  };

  using MudSystemPtr = shared_ptr<MudSystem>;

} // namespace primordia::mud::system