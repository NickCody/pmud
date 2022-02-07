#pragma once
#include <string>
#include "caf/all.hpp"


namespace primordia::mud {

  using namespace caf;
  
  struct MudConfig {
    std::string name;
    std::string address;
    uint16_t port;
    uint16_t max_queued_connections;
  };

  struct ServerState {
    MudConfig config;
    // sockaddr_in sockaddr;
    int sockfd;
  };

  struct ConnectionState {
    int connection;
    std::string registery_id;
    int break_count;
    std::string current_input;
  };
} // namespace primordia::mud

CAF_BEGIN_TYPE_ID_BLOCK(primorda_mud_caf_types, first_custom_type_id)

CAF_ADD_TYPE_ID(primorda_mud_caf_types, (primordia::mud::MudConfig))
CAF_ADD_TYPE_ID(primorda_mud_caf_types, (primordia::mud::ServerState))
CAF_ADD_TYPE_ID(primorda_mud_caf_types, (primordia::mud::ConnectionState))

CAF_ADD_ATOM(primorda_mud_caf_types, AcceptConnection)
CAF_ADD_ATOM(primorda_mud_caf_types, StartServer)
CAF_ADD_ATOM(primorda_mud_caf_types, GoodbyeServer)
CAF_ADD_ATOM(primorda_mud_caf_types, WaitForInput)
CAF_ADD_ATOM(primorda_mud_caf_types, PromptUser)
CAF_ADD_ATOM(primorda_mud_caf_types, CloseConnection)

CAF_END_TYPE_ID_BLOCK(primorda_mud_caf_types)

namespace primordia::mud {

  template <class Inspector> bool inspect(Inspector& f, MudConfig& x) {

    return f.object(x).fields(
        f.field("name", x.name), f.field("address", x.address), f.field("port", x.port), f.field("max_queued_connections", x.max_queued_connections));
  }

  template <class Inspector> bool inspect(Inspector& f, ServerState& x) {

    return f.object(x).fields(f.field("config", x.config), f.field("sockfd", x.sockfd));
  }

  template <class Inspector> bool inspect(Inspector& f, ConnectionState& x) {

    return f.object(x).fields(f.field("connection", x.connection),
                              f.field("registery_id", x.registery_id),
                              f.field("break_count", x.break_count),
                              f.field("current_input", x.current_input));
  }
} // namespace primordia::mud