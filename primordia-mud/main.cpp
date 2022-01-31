#include <csignal>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include <regex>
#include "caf/all.hpp"
#include "comm/comm_static.h"

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
// Some references:
// simple telnet server: https://ncona.com/2019/04/building-a-simple-server-with-cpp/
// telnet and ncurses: https://stackoverflow.com/questions/45325591/ncurses-telnet-protocol
// RFC:
//   - https://www.rfc-editor.org/rfc/rfc854
//   - https://www.rfc-editor.org/rfc/rfc5198
//   - https://www.rfc-editor.org/rfc/rfc856
//

std::string YAML_CONFIG = "primordia-mud.yaml";

struct MudConfig {
  std::string name;
  std::string address;
  uint16_t port;
};

struct ServerState {
  MudConfig config;
  // sockaddr_in sockaddr;
  int sockfd;
};

struct ConnectionState {
  int connection;
  std::string registery_id;
};

CAF_BEGIN_TYPE_ID_BLOCK(primorda_mud_caf_types, first_custom_type_id)

CAF_ADD_TYPE_ID(primorda_mud_caf_types, (MudConfig))
CAF_ADD_TYPE_ID(primorda_mud_caf_types, (ServerState))
CAF_ADD_TYPE_ID(primorda_mud_caf_types, (ConnectionState))

CAF_ADD_ATOM(primorda_mud_caf_types, AcceptConnection)
CAF_ADD_ATOM(primorda_mud_caf_types, StartServer)
CAF_ADD_ATOM(primorda_mud_caf_types, GoodbyeServer)
CAF_ADD_ATOM(primorda_mud_caf_types, WaitForInput)
CAF_ADD_ATOM(primorda_mud_caf_types, PromptUser)
CAF_ADD_ATOM(primorda_mud_caf_types, CloseConnection)

CAF_END_TYPE_ID_BLOCK(primorda_mud_caf_types)

template <class Inspector> bool inspect(Inspector& f, MudConfig& x) {

  return f.object(x).fields(f.field("name", x.name), f.field("address", x.address), f.field("port", x.port));
}

template <class Inspector> bool inspect(Inspector& f, ServerState& x) {

  return f.object(x).fields(f.field("config", x.config), f.field("sockfd", x.sockfd));
}

template <class Inspector> bool inspect(Inspector& f, ConnectionState& x) {

  return f.object(x).fields(f.field("connection", x.connection), f.field("registery_id", x.registery_id));
}

caf::behavior Connection(caf::stateful_actor<ConnectionState>* self, const std::string welcome, int connection) {
  aout(self) << "ctor\n";
  self->state.connection = connection;
  self->state.registery_id = fmt::format("Connection({})", self->id());
  self->system().registry().put(self->state.registery_id, self);

  {
    CommStatic comm(self->state.connection);
    bool success = comm.emit_banner() && comm.emit_line() && comm.emit_line(welcome) && comm.emit_line() && comm.emit_line();
    if (!success) {
      aout(self) << fmt::format("Failed to send welcome to connection {}\n", self->state.connection);
      self->send(self, CloseConnection_v);
    }
  }

  aout(self) << fmt::format("Was able to send banner!\n", self->state.connection);

  return {
    [=](PromptUser) {
      CommStatic comm(self->state.connection);
      comm.emit_prompt();
      self->send(self, WaitForInput_v);
    },
    [=](WaitForInput) {
      CommStatic comm(self->state.connection);
      if (comm.has_data()) {
        ssize_t bytes_read = 0;
        std::string user_read = comm.read_from_user(bytes_read);
        aout(self) << fmt::format("User entered: {}\n", user_read);
        if (bytes_read == 0) {
          aout(self) << fmt::format("Connection {} quit\n", connection);
          comm.emit_line();
          close(self->state.connection);
          self->state.connection = -1;
          self->system().registry().erase(self->state.registery_id);
          self->quit();
        } else {
          std::string response = fmt::format("You said {}", user_read);
          comm.emit_line(response);
          self->send(self, PromptUser_v);
        }
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        self->send(self, WaitForInput_v);
      }
    },
    [=](CloseConnection) {
      aout(self) << fmt::format("Connection terminating: {}", self->state.connection);
      if (connection != -1)
        close(connection);
      self->state.connection = -1;
      self->quit();
    },
  };
}

void initialize_sockaddr(const std::string& address, uint16_t port, sockaddr_in& sa) {
  sa.sin_family = AF_INET;
  inet_aton(address.c_str(), &sa.sin_addr);
  sa.sin_port = htons(port);
  memset(&sa.sin_zero, 0, sizeof(sa.sin_zero));
}

caf::behavior Server(caf::stateful_actor<ServerState>* self, const MudConfig& _config) {
  self->state.config = _config;
  self->state.sockfd = 0;

  return {
    [=](StartServer) -> bool {
      aout(self) << fmt::format("{} server starting up...\n", self->state.config.name);

      self->state.sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (self->state.sockfd == -1) {
        aout(self) << fmt::format("Failed to create socket. errno: {}\n", errno);
        return false;
      }

      int flags = fcntl(self->state.sockfd, F_GETFL);
      fcntl(self->state.sockfd, F_SETFL, flags | O_NONBLOCK);

      sockaddr_in sockaddr;
      initialize_sockaddr(self->state.config.address.c_str(), self->state.config.port, sockaddr);

      if (bind(self->state.sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
        aout(self) << fmt::format("Failed to bind to port {}. errno: {}\n", self->state.config.port, errno);
        return false;
      }

      char sockaddr_buffer[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &sockaddr.sin_addr, sockaddr_buffer, sizeof(sockaddr_buffer));
      aout(self) << fmt::format("Listening on {}:{}\n", sockaddr_buffer, self->state.config.port);

      // Start listening. Hold at most 10 connections in the queue
      if (listen(self->state.sockfd, 2) < 0) {
        aout(self) << fmt::format("Failed to listen on socket. errno: {}\n", errno);
        return false;
      }

      aout(self) << fmt::format("Listening on sockfd: {}\n", self->state.sockfd);
      self->send(self, AcceptConnection_v);

      return true;
    },
    [=](AcceptConnection) {
      if (self->state.sockfd <= 0)
        return;

      sockaddr_in sockaddr;
      initialize_sockaddr(self->state.config.address.c_str(), self->state.config.port, sockaddr);

      auto addrlen = sizeof(sockaddr);
      int c_id = accept(self->state.sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
      if (c_id == -1) {
        if (errno == EWOULDBLOCK || errno == EAGAIN) {
          std::this_thread::sleep_for(std::chrono::milliseconds(20));
        } else {
          aout(self) << fmt::format("Failed to grab connection. errno: {}\n", errno);
        }
      } else {

        int flags = fcntl(c_id, F_GETFL);
        fcntl(c_id, F_SETFL, flags | O_NONBLOCK);

        aout(self) << fmt::format("Sending welcome to {}\n", c_id);
        std::string welcome = fmt::format("Welcome to {}", self->state.config.name);
        auto connection_actor = self->spawn(Connection, welcome, c_id);
        self->send(connection_actor, PromptUser_v);
      }

      self->send(self, AcceptConnection_v);
    },
    [=](GoodbyeServer) -> bool {
      aout(self) << fmt::format("Server actor terminating...\n");
      close(self->state.sockfd);
      self->state.sockfd = 0;
      self->quit();
      return true;
    },
  };
}

// -==---=-=-=-=-=-=-=-=-=-=--===-=-==-=-=-=--==-=-===-=-=-=-=-=-=-=-=-==-=-=-=
MudConfig parse_yaml(const std::string& filename) {

  const YAML::Node config = YAML::LoadFile(filename);

  return {
    config["server"]["name"].as<std::string>(),
    config["server"]["address"].as<std::string>(),
    config["server"]["port"].as<uint16_t>(),
  };
}

namespace {
  volatile std::sig_atomic_t g_signal_status = 0;
}

void signal_handler(int signal) {
  if (signal == SIGTERM && g_signal_status == SIGTERM) {
    exit(-1);
  } else {
    g_signal_status = signal;
  }
}

void caf_main(caf::actor_system& sys) {
  std::signal(SIGINT, signal_handler);

  YAML_CONFIG = get_or(sys.config(), "primordia-mud.yaml_config", YAML_CONFIG);
  MudConfig config = parse_yaml(YAML_CONFIG);

  {
    caf::scoped_actor self{ sys };

    auto server = sys.spawn(Server, config);

    bool server_success = false;
    self->request(server, std::chrono::seconds(10), StartServer_v)
        .receive([&](bool status) { server_success = status; },
                 [&](const caf::error& err) { aout(self) << fmt::format("Error: {}\n", to_string(err)); });

    while (server_success && g_signal_status != SIGINT) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    for (auto actor_in_registry : sys.registry().named_actors()) {
      std::regex connection_regex("Connection\\([0-9]+\\)");
      if (std::regex_match(actor_in_registry.first, connection_regex)) {
        fmt::print("Forcing connection to close: {}\n", actor_in_registry.first);
        self->send(caf::actor_cast<caf::actor>(actor_in_registry.second), CloseConnection_v);
        self->send_exit(actor_in_registry.second, caf::exit_reason::user_shutdown);
      }
    }

    self->request(server, std::chrono::seconds(10), GoodbyeServer_v)
        .receive([&](bool /*status*/) {}, [&](const caf::error& err) { aout(self) << fmt::format("Error: {}\n", to_string(err)); });

    self->send_exit(server, caf::exit_reason::user_shutdown);
  }

  fmt::print("Waiting for actors to terminate...\n", sys.registry().running());
  fmt::print("Done\n");
}

// creates a main function for us that calls our caf_main
CAF_MAIN(caf::id_block::primorda_mud_caf_types)