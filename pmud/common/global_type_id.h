#pragma once

#include "caf/type_id.hpp"

namespace primordia::mud::type_id {

  constexpr caf::type_id_t first_net_type_id = caf::first_custom_type_id;
  constexpr caf::type_id_t first_player_type_id = first_net_type_id + 200;

} // namespace primordia::mud::type_id
