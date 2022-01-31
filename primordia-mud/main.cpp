#include <csignal>
#include <cstdlib>
#include <unistd.h>
#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include <regex>
#include "caf/all.hpp"

#include "comm/comm_static.h"
#include "comm/server_state.h"
#include "comm/connection.h"
#include "comm/server.h"
#include "comm/util.h"

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
// Some references:
// simple telnet server: https://ncona.com/2019/04/building-a-simple-server-with-cpp/
// telnet and ncurses: https://stackoverflow.com/questions/45325591/ncurses-telnet-protocol
// RFC:
//   - https://www.rfc-editor.org/rfc/rfc854
//   - https://www.rfc-editor.org/rfc/rfc5198
//   - https://www.rfc-editor.org/rfc/rfc856
//

using namespace primordia::mud;
using namespace std;
using namespace caf;
using namespace fmt;

string YAML_CONFIG = "primordia-mud.yaml";

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
MudConfig parse_yaml(const string& filename) {

  const YAML::Node config = YAML::LoadFile(filename);

  return {
    config["server"]["name"].as<string>(),
    config["server"]["address"].as<string>(),
    config["server"]["port"].as<uint16_t>(),
  };
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
namespace {
  volatile sig_atomic_t g_signal_status = 0;
}

void signal_handler(int signal) {
  if (signal == SIGTERM && g_signal_status == SIGTERM) {
    exit(-1);
  } else {
    g_signal_status = signal;
  }
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
bool start_server(scoped_actor& self, const actor& server, chrono::seconds timeout) {
  bool server_success = false;
  self->request(server, timeout, StartServer_v)
      .receive([&](bool status) { server_success = status; }, [&](const error& err) { aout(self) << format("Error: {}\n", to_string(err)); });

  return server_success;
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
void quit_connection_actors(scoped_actor& self, actor_system& sys) {
  for (auto actor_in_registry : sys.registry().named_actors()) {
    regex connection_regex("Connection\\([0-9]+\\)");
    if (regex_match(actor_in_registry.first, connection_regex)) {
      print("Forcing connection to close: {}\n", actor_in_registry.first);
      self->send(actor_cast<actor>(actor_in_registry.second), CloseConnection_v);
      self->send_exit(actor_in_registry.second, exit_reason::user_shutdown);
    }
  }
}

void kill_server(scoped_actor& self, const actor& server, chrono::seconds timeout) {
  self->request(server, timeout, GoodbyeServer_v)
      .receive([&](bool /*status*/) {}, [&](const error& err) { aout(self) << format("Error: {}\n", to_string(err)); });
  self->send_exit(server, exit_reason::user_shutdown);
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
void caf_main(actor_system& sys) {
  signal(SIGINT, signal_handler);

  YAML_CONFIG = get_or(sys.config(), "primordia-mud.yaml_config", YAML_CONFIG);
  MudConfig config = parse_yaml(YAML_CONFIG);

  {
    scoped_actor self{ sys };
    auto server = sys.spawn(Server, config);

    bool server_success = start_server(self, server, chrono::seconds(10));

    while (server_success && g_signal_status != SIGINT) {
      this_thread::sleep_for(chrono::milliseconds(500));
    }

    quit_connection_actors(self, sys);

    kill_server(self, server, chrono::seconds(10));
  }

  print("Done\n");
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
CAF_MAIN(id_block::primorda_mud_caf_types)