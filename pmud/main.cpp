#include <csignal>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <regex>

#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include <fmt/core.h>
#include <yaml-cpp/yaml.h>

#include "caf/all.hpp"

#include "pnet/comm_static.h"
#include "pnet/server_state.h"
#include "pnet/connection.h"
#include "pnet/server.h"
#include "pnet/util.h"
#include "storage/storage_actor.h"
#include "spdlog/spdlog.h"
#include "player/player_type_id.h"
#include "system/pmud_system.h"
#include "storage/redis_storage.h"

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
// Some references:
// simple telnet server: https://ncona.com/2019/04/building-a-simple-server-with-cpp/
// telnet and ncurses: https://stackoverflow.com/questions/45325591/ncurses-telnet-protocol
// RFC:
//   - https://www.rfc-editor.org/rfc/rfc854
//   - https://www.rfc-editor.org/rfc/rfc5198
//   - https://www.rfc-editor.org/rfc/rfc856
//
// Coping with the TCP TIME_WAIT state
//  - https://vincent.bernat.ch/en/blog/2014-tcp-time-wait-state-linux
//

using namespace primordia::mud;
using namespace std;
using namespace caf;
using namespace fmt;
using namespace primordia::mud::storage;
using namespace primordia::mud::system;
using namespace primordia::mud::pnet;

namespace redis_storage = primordia::mud::storage::redis;

string YAML_CONFIG = "primordia-mud.yaml";

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
MudConfig parse_yaml(const string& filename) {
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    SPDLOG_INFO("Loading yaml {} from current working dir: {}", filename, cwd);
  }
  const YAML::Node config = YAML::LoadFile(filename);

  return {
    config["server"]["name"].as<string>(),
    config["server"]["address"].as<string>(),
    config["server"]["port"].as<uint16_t>(),
    config["server"]["max_queued_connections"].as<uint16_t>(),
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
int start_server(scoped_actor& self, const actor& server, chrono::seconds timeout) {
  int server_success = 0;
  self->request(server, timeout, StartServer_v)
      .receive([&](int status) { server_success = status; }, [&](const error& err) { SPDLOG_INFO("Error: {}", to_string(err)); });

  return server_success;
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
void quit_connection_actors(scoped_actor& self, actor_system& sys) {
  for (auto actor_in_registry : sys.registry().named_actors()) {
    regex connection_regex("Connection\\([0-9]+\\)");
    if (regex_match(actor_in_registry.first, connection_regex)) {
      SPDLOG_INFO("App shutdown, forcing actor {} to close", actor_in_registry.first);
      self->send(actor_cast<actor>(actor_in_registry.second), GoodbyeConnection_v);
    }
  }
}

void kill_server(scoped_actor& self, const actor& server, chrono::seconds timeout) {
  self->request(server, timeout, GoodbyeServer_v)
      .receive([&](bool status) { SPDLOG_INFO("Server exit with status: {}", status); },
               [&](const error& err) { aout(self) << format("Error: {}", to_string(err)); });
}

void run(actor_system& sys, MudSystemPtr mud) {
  scoped_actor self{ sys };

  auto server = sys.spawn<Server>(mud);

  int server_status = start_server(self, server, chrono::seconds(10));
  if (server_status != 0) {
    SPDLOG_INFO("Server failed to start!");
    exit(server_status);
  }

  while (g_signal_status != SIGINT) {
    this_thread::sleep_for(chrono::milliseconds(500));
  }

  quit_connection_actors(self, sys);

  kill_server(self, server, chrono::seconds(10));
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
void caf_main(actor_system& sys) {
  signal(SIGINT, signal_handler);

  YAML_CONFIG = get_or(sys.config(), "pmud.yaml_config", YAML_CONFIG);
  MudConfig config = parse_yaml(YAML_CONFIG);

  unique_ptr<Storage> storage = redis_storage::initialize_redis_storage();
  if (!storage) {
    SPDLOG_ERROR("Exiting due to storage initialization failure!");
    exit(-1);
  }

  {
    auto storage_actor = sys.spawn<StorageActor>(move(storage));

    MudSystemPtr mud_system = make_shared<MudSystem>(sys, config, actor_cast<strong_actor_ptr>(storage_actor));

    run(sys, mud_system);
  }

  sys.await_all_actors_done();

  SPDLOG_INFO("Exiting main");
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
CAF_MAIN(id_block::primorda_mud_caf_types, id_block::player_caf_types)