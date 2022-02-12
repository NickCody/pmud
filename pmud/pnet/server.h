#pragma once

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/format.h>

#include "caf/stateful_actor.hpp"

#include "util.h"

namespace primordia::mud {

  using namespace fmt;
  using namespace caf;
  using namespace std;

  behavior Server(stateful_actor<ServerState>* self, const MudConfig& _config) {
    self->state.config = _config;
    self->state.sockfd = 0;

    return {
      [=](StartServer) -> int {
        LOG_INFO("{} server starting up...", self->state.config.name);

        self->state.sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (self->state.sockfd == -1) {
          LOG_INFO("Failed to create socket. errno: {}", errno);
          return errno;
        }

        // set socket to non-blocking
        //
        int flags = fcntl(self->state.sockfd, F_GETFL);
        fcntl(self->state.sockfd, F_SETFL, flags | O_NONBLOCK);

        sockaddr_in sockaddr;
        initialize_sockaddr(self->state.config.address.c_str(), self->state.config.port, sockaddr);

        if (bind(self->state.sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
          LOG_INFO("Failed to bind to port {}. errno: {}", self->state.config.port, errno);
          return errno;
        }

        char sockaddr_buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sockaddr.sin_addr, sockaddr_buffer, sizeof(sockaddr_buffer));
        LOG_INFO("Listening on {}:{}", sockaddr_buffer, self->state.config.port);

        // Start listening. Hold at most 10 connections in the queue
        if (listen(self->state.sockfd, self->state.config.max_queued_connections) < 0) {
          LOG_INFO("Failed to listen on socket. errno: {}", errno);
          return errno;
        }

        LOG_INFO("Listening on sockfd: {}", self->state.sockfd);
        self->send(self, AcceptConnection_v);

        return 0;
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
            this_thread::sleep_for(chrono::milliseconds(20));
          } else {
            LOG_INFO("Failed to grab connection. errno: {}", errno);
          }
        } else {

          int flags = fcntl(c_id, F_GETFL);
          fcntl(c_id, F_SETFL, flags | O_NONBLOCK);

          LOG_INFO("Sending welcome to {}", c_id);
          string welcome = format("Welcome to {}\nVersion 0.1", self->state.config.name);
          auto connection_actor = self->spawn(Connection, welcome, c_id);
          self->send(connection_actor, Welcome_v);
        }

        self->send(self, AcceptConnection_v);
      },
      [=](GoodbyeServer) -> bool {
        LOG_INFO_1("Server actor terminating...");
        close(self->state.sockfd);
        self->state.sockfd = 0;
        self->quit();
        return true;
      },
    };
  }
} // namespace primordia::mud