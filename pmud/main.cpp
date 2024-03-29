#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <csignal>
#include <fmt/core.h>
#include <yaml-cpp/yaml.h>
#include <caf/all.hpp>

#include "common/global_type_id.h"

#include "pnet/server_actor.h"
#include "storage/storage_actor.h"
#include "system/pmud_system.h"
#include "storage/redis_storage.h"
#include "event/event_recorder_actor.h"

using namespace std;
using namespace caf;
using namespace fmt;
using namespace primordia::mud::storage;
using namespace primordia::mud::system;
using namespace primordia::mud::pnet;
using namespace primordia::mud::event;

namespace redis_storage = primordia::mud::storage::redis;

string YAML_CONFIG = "primordia-mud.yaml";

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
MudConfig parse_yaml(const string& filename) {
  const YAML::Node config = YAML::LoadFile(filename);

  return {
    config["server"]["name"].as<string>(),         config["server"]["address"].as<string>(),
    config["server"]["port"].as<uint16_t>(),       config["server"]["max-queued-connections"].as<uint16_t>(),
    config["server"]["log-name"].as<string>(),     config["server"]["log-level"].as<string>(),
    config["server"]["log-filename"].as<string>(), config["server"]["log-truncate"].as<bool>(),
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
  self->request(server, timeout, StartServer())
      .receive([&](int status) { server_success = status; }, [&](const error& err) { spdlog::info("Error: {}", to_string(err)); });

  return server_success;
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
void quit_connection_actors(scoped_actor& self, actor_system& sys) {
  for (auto actor_in_registry : sys.registry().named_actors()) {
    regex connection_regex("Connection\\([0-9]+\\)");
    if (regex_match(actor_in_registry.first, connection_regex)) {
      spdlog::info("App shutdown, forcing actor {} to close", actor_in_registry.first);
      self->send(actor_cast<actor>(actor_in_registry.second), GoodbyeConnection());
    }
  }
}

/**
 * \brief Kills server actor
 *
 * Kills the tcp server actor hosting connections
 *
 * @param self Scope actor used for send/response
 * @param server Server actor
 * @param timeout timeout before server kill fails
 * @return true if server actor was terminated, otherwise false
 *
 */

bool kill_server(scoped_actor& self, const actor& server, chrono::seconds timeout) {
  bool result;

  self->request(server, timeout, GoodbyeServer())
      .receive(
          [&](bool status) {
            spdlog::info("Server exit with status: {}", status);
            result = true;
          },
          [&](const error& err) {
            spdlog::error("Error killing server: {}", to_string(err));
            result = false;
          });

  return result;
}

/**
 * \brief main event loop (does nothing)
 *
 * This is the main thread, which does nothing at the moment other than loop and wait for a signal.
 *
 * @param sys CAF Actor System
 * @param mud MUD system
 *
 */

bool run(actor_system& sys, MudSystemPtr mud) {
  scoped_actor self{ sys };

  auto server = sys.spawn<ServerActor>(mud);

  int server_status = start_server(self, server, chrono::seconds(10));
  if (server_status != 0) {
    spdlog::error("Server failed to start!");
    return false;
  }

  spdlog::info("Server started successfully");

  while (g_signal_status != SIGINT) {
    this_thread::sleep_for(chrono::milliseconds(50));
  }

  quit_connection_actors(self, sys);

  return kill_server(self, server, chrono::seconds(10));
}

/**
 * \brief Initializes the logger
 *
 * Longer Description
 *
 * @param config MudConfig which contains logging options
 * @returns true if logger was initialized, false otherwise
 *
 */

bool configure_logging(const MudConfig& config) {

  try {
    std::shared_ptr<spdlog::logger> logger;

    if (config.log_name == "console") {
      fmt::print("Configuring console logger...\n");
      logger = spdlog::stdout_color_mt<spdlog::async_factory>("console", spdlog::color_mode::automatic);
    } else {
      fmt::print("Configuring file logger to {}...\n", config.log_filename);
      logger = spdlog::basic_logger_mt<spdlog::async_factory>(config.log_name, config.log_filename, config.log_truncate);
    }
    logger->set_level(spdlog::level::from_str(config.log_level));
    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::info);
    spdlog::info("Logging configured successfully!");
    return true;
  } catch (const spdlog::spdlog_ex& ex) {
    fmt::print("Log init failed: {}\n", ex.what());
    return false;
  }
}

/**
 * \brief main function
 *
 * This is  the main entrypoint provided by CAF actor system.
 *
 * @param sys CAF Actor System
 *
 */
void caf_main(actor_system& sys) {
  signal(SIGINT, signal_handler);

  auto [argc, argv] = sys.config().c_args_remainder();

  if (argc > 1) {
    YAML_CONFIG = argv[1];
  }

  MudConfig config = parse_yaml(YAML_CONFIG);

  if (!configure_logging(config)) {
    exit(-1);
  }

  unique_ptr<Storage> storage = redis_storage::initialize_redis_storage();
  if (!storage) {
    spdlog::error("Exiting due to storage initialization failure!");
    exit(-1);
  }

  bool kill_result;

  {
    auto storage_actor = actor_cast<strong_actor_ptr>(sys.spawn<StorageActor>(std::move(storage)));
    auto event_actor = actor_cast<strong_actor_ptr>(sys.spawn<EventRecorderActor>(storage_actor));

    MudSystemPtr mud_system = make_shared<MudSystem>(sys, config, storage_actor, event_actor);

    kill_result = run(sys, mud_system);
  }

  if (kill_result) {
    sys.await_all_actors_done();
  }

  spdlog::info("Exiting main");
  fmt::print("Exiting main\n");
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
//
CAF_MAIN(id_block::pmud_caf_types)