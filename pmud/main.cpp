#include <csignal>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include <regex>
#include "caf/all.hpp"

#include "pnet/comm_static.h"
#include "pnet/server_state.h"
#include "pnet/connection.h"
#include "pnet/server.h"
#include "pnet/util.h"
#include "storage/storage.h"
#include "logger/logger.h"

#include "player/player_type_id.h"

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

string YAML_CONFIG = "primordia-mud.yaml";

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
MudConfig parse_yaml(const string& filename) {

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
      .receive([&](int status) { server_success = status; }, [&](const error& err) { LOG_INFO("Error: {}", to_string(err)); });

  return server_success;
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
void quit_connection_actors(scoped_actor& self, actor_system& sys) {
  for (auto actor_in_registry : sys.registry().named_actors()) {
    regex connection_regex("Connection\\([0-9]+\\)");
    if (regex_match(actor_in_registry.first, connection_regex)) {
      LOG_INFO("Forcing connection to close: {}", actor_in_registry.first);
      self->send(actor_cast<actor>(actor_in_registry.second), GoodbyeConnection_v);
      self->send_exit(actor_in_registry.second, exit_reason::user_shutdown);
    }
  }
}

void kill_server(scoped_actor& self, const actor& server, chrono::seconds timeout) {
  self->request(server, timeout, GoodbyeServer_v)
      .receive([&](bool /*status*/) {}, [&](const error& err) { aout(self) << format("Error: {}", to_string(err)); });
  self->send_exit(server, exit_reason::user_shutdown);
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
void caf_main(actor_system& sys) {
  scoped_actor self{ sys };
  primordia::mud::logger::Logger::init(&self);

  signal(SIGINT, signal_handler);

  YAML_CONFIG = get_or(sys.config(), "primordia-mud.yaml_config", YAML_CONFIG);
  MudConfig config = parse_yaml(YAML_CONFIG);

  char* e_redis_host = getenv("REDIS_HOST");
  char* e_redis_port = getenv("REDIS_PORT");
  char* e_env = getenv("ENV");

  if (!e_redis_host || !e_redis_port) {
    LOG_INFO_1("Error, redis host and port should be specified with environment REDIS_HOST and REDIS_PORT");
    exit(1);
  }

  if (!e_env) {
    LOG_INFO_1("Error, ENV needs to be set!");
    exit(1);
  }

  const string& redis_host = e_redis_host;
  uint16_t redis_port = (uint16_t)stoul(e_redis_port);
  primordia::mud::storage::RedisStorage storage(redis_host, redis_port);

  if (!storage.init(e_env)) {
    LOG_INFO("Error, could not connect to REDIS_HOST {} on REDIS_PORT {}", redis_host, redis_port);
    exit(1);
  }

  LOG_INFO("Successsfully connected to redis at {}:{}!", redis_host, redis_port);

  auto server = sys.spawn(Server, config);

  int server_status = start_server(self, server, chrono::seconds(10));
  if (server_status != 0) {
    LOG_INFO_1("Server failed to start!");
    exit(server_status);
  }

  while (g_signal_status != SIGINT) {
    this_thread::sleep_for(chrono::milliseconds(500));
  }

  quit_connection_actors(self, sys);

  kill_server(self, server, chrono::seconds(10));

  LOG_INFO_1("Done");
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
CAF_MAIN(id_block::primorda_mud_caf_types, id_block::player_caf_types)