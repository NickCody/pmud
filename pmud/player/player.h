#pragma once

#include <string>

#include "caf/event_based_actor.hpp"

namespace primordia::mud::player {

  using namespace caf;

  class Player : public event_based_actor {
    string login_id;

  public:
    Player(actor_config& config)
        : event_based_actor(cfg) {
    }

    behavior make_behavior() override {
      return {
      }
    }
  };

} // namespace primordia::mud::player