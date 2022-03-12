#pragma once

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/core.h>

#include "caf/stateful_actor.hpp"

#include "util.h"
#include "system/pmud_system.h"

namespace primordia::mud {

  using namespace fmt;
  using namespace caf;
  using namespace std;

  class Server : public event_based_actor {
  public:
    Server(actor_config& cfg, MudSystemPtr mud)
        : event_based_actor(cfg) {
      state.sockfd = 0;
      m_mud = mud;
    }

    behavior make_behavior() {
      return {
        [this](StartServer) -> int {
          SPDLOG_INFO("{} server starting up...", m_mud->get_config().name);

          state.sockfd = socket(AF_INET, SOCK_STREAM, 0);
          if (state.sockfd == -1) {
            SPDLOG_INFO("Failed to create socket. errno: {}", errno);
            return errno;
          }

          // set socket to non-blocking
          //
          int flags = fcntl(state.sockfd, F_GETFL);
          fcntl(state.sockfd, F_SETFL, flags | O_NONBLOCK);

          sockaddr_in sockaddr;
          initialize_sockaddr(m_mud->get_config().address.c_str(), m_mud->get_config().port, sockaddr);

          if (bind(state.sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
            SPDLOG_INFO("Failed to bind to port {}. errno: {}", m_mud->get_config().port, errno);
            return errno;
          }

          char sockaddr_buffer[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &sockaddr.sin_addr, sockaddr_buffer, sizeof(sockaddr_buffer));
          SPDLOG_INFO("Listening on {}:{}", sockaddr_buffer, m_mud->get_config().port);

          // Start listening. Hold at most 10 connections in the queue
          if (listen(state.sockfd, m_mud->get_config().max_queued_connections) < 0) {
            SPDLOG_INFO("Failed to listen on socket. errno: {}", errno);
            return errno;
          }

          SPDLOG_INFO("Listening on sockfd: {}", state.sockfd);
          send(this, AcceptConnection_v);

          return 0;
        },
        [this](AcceptConnection) {
          if (state.sockfd <= 0)
            return;

          sockaddr_in sockaddr;
          initialize_sockaddr(m_mud->get_config().address.c_str(), m_mud->get_config().port, sockaddr);

          auto addrlen = sizeof(sockaddr);
          int c_id = accept(state.sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
          if (c_id == -1) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
              this_thread::sleep_for(chrono::milliseconds(20));
            } else {
              SPDLOG_INFO("Failed to grab connection. errno: {}", errno);
            }
          } else {

            int flags = fcntl(c_id, F_GETFL);
            fcntl(c_id, F_SETFL, flags | O_NONBLOCK);

            SPDLOG_INFO("Performing welcome for connection {}", c_id);
            string welcome = format("Welcome to {}\nVersion 0.1", m_mud->get_config().name);
            auto connection_actor = spawn<Connection>(m_mud, welcome, c_id);
            send(connection_actor, PerformWelcome_v);
          }

          send(this, AcceptConnection_v);
        },
        [this](GoodbyeServer) -> bool {
          SPDLOG_INFO("Server actor terminating...");
          close(state.sockfd);
          state.sockfd = 0;
          send_exit(actor_cast<actor>(this), exit_reason::user_shutdown);
          return true;
        },
      };
    };

  private:
    ServerState state;
    MudSystemPtr m_mud;
  };

} // namespace primordia::mud