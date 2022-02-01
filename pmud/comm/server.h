#pragma once

#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/format.h>

#include "caf/all.hpp"

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
        aout(self) << format("{} server starting up...\n", self->state.config.name);

        self->state.sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (self->state.sockfd == -1) {
          aout(self) << format("Failed to create socket. errno: {}\n", errno);
          return errno;
        }

        // set socket to non-blocking
        //
        int flags = fcntl(self->state.sockfd, F_GETFL);
        fcntl(self->state.sockfd, F_SETFL, flags | O_NONBLOCK);

        sockaddr_in sockaddr;
        initialize_sockaddr(self->state.config.address.c_str(), self->state.config.port, sockaddr);

        if (bind(self->state.sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
          aout(self) << format("Failed to bind to port {}. errno: {}\n", self->state.config.port, errno);
          return errno;
        }

        char sockaddr_buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sockaddr.sin_addr, sockaddr_buffer, sizeof(sockaddr_buffer));
        aout(self) << format("Listening on {}:{}\n", sockaddr_buffer, self->state.config.port);

        // Start listening. Hold at most 10 connections in the queue
        if (listen(self->state.sockfd, self->state.config.max_queued_connections) < 0) {
          aout(self) << format("Failed to listen on socket. errno: {}\n", errno);
          return errno;
        }

        aout(self) << format("Listening on sockfd: {}\n", self->state.sockfd);
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
            aout(self) << format("Failed to grab connection. errno: {}\n", errno);
          }
        } else {

          int flags = fcntl(c_id, F_GETFL);
          fcntl(c_id, F_SETFL, flags | O_NONBLOCK);

          aout(self) << format("Sending welcome to {}\n", c_id);
          string welcome = format("Welcome to {}", self->state.config.name);
          auto connection_actor = self->spawn(Connection, welcome, c_id);
          self->send(connection_actor, PromptUser_v);
        }

        self->send(self, AcceptConnection_v);
      },
      [=](GoodbyeServer) -> bool {
        aout(self) << format("Server actor terminating...\n");
        close(self->state.sockfd);
        self->state.sockfd = 0;
        self->quit();
        return true;
      },
    };
  }
} // namespace primordia::mud