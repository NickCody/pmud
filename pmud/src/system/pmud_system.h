#pragma once

#include <caf/all.hpp>

#include "system/mud_config.h"

namespace primordia::mud::system {

using namespace std;
using namespace caf;
using namespace primordia::mud;

class MudSystem {
public:
  MudSystem(actor_system &caf_system, MudConfig &mud_config,
            strong_actor_ptr storage_actor,
            strong_actor_ptr event_recorder_actor)
      : m_actor_system(caf_system), m_mud_config(mud_config),
        m_storage_actor(storage_actor),
        m_event_recorder_actor(event_recorder_actor) {}

  ~MudSystem() {
    caf::anon_send_exit(m_storage_actor, caf::exit_reason::user_shutdown);
    caf::anon_send_exit(m_event_recorder_actor,
                        caf::exit_reason::user_shutdown);
  }

  actor_system &actorsystem() { return m_actor_system; }

  const MudConfig &get_config() { return m_mud_config; }

  strong_actor_ptr get_storage_actor() { return m_storage_actor; }

  strong_actor_ptr get_event_recorder_actor() { return m_event_recorder_actor; }

private:
  actor_system &m_actor_system;
  MudConfig m_mud_config;
  strong_actor_ptr m_storage_actor;
  strong_actor_ptr m_event_recorder_actor;
};

using MudSystemPtr = shared_ptr<MudSystem>;

} // namespace primordia::mud::system