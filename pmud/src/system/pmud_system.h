#pragma once

#include <caf/actor_system.hpp>
#include "storage/storage.h"
#include "pnet/server_state.h"

namespace primordia::mud::system {

  using namespace std;
  using namespace caf;
  using namespace primordia::mud;
  using namespace primordia::mud::pnet;

  class MudSystem {
  public:
    MudSystem(actor_system& caf_system, MudConfig& mud_config, strong_actor_ptr storage_actor)
        : m_actor_system(caf_system),
          m_mud_config(mud_config),
          m_storage_actor(storage_actor) {}

    ~MudSystem() {
      m_storage_actor.reset();
    }

    actor_system& actorsystem() {
      return m_actor_system;
    }

    const MudConfig& get_config() {
      return m_mud_config;
    }

    strong_actor_ptr get_storage() {
      return m_storage_actor;
    }

  private:
    actor_system& m_actor_system;
    MudConfig m_mud_config;
    strong_actor_ptr m_storage_actor;
  };

  using MudSystemPtr = shared_ptr<MudSystem>;

} // namespace primordia::mud::system