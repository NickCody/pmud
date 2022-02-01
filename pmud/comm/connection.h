#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/format.h>

#include "caf/all.hpp"

#include "server_state.h"

namespace primordia::mud {

  using namespace fmt;
  using namespace caf;
  using namespace std;

  behavior Connection(stateful_actor<ConnectionState>* self, const string welcome, int connection) {
    self->state.connection = connection;
    self->state.break_count = 0;
    self->state.current_input = "";

    self->state.registery_id = format("Connection({})", self->id());
    self->system().registry().put(self->state.registery_id, self);

    {
      CommStatic comm(self->state.connection);
      bool success = comm.emit_banner() && comm.emit_line() && comm.emit_line(welcome) && comm.emit_line() && comm.emit_line();
      if (!success) {
        aout(self) << format("Failed to send welcome to connection {}\n", self->state.connection);
        // self->send(self, CloseConnection_v);
      }
    }

    aout(self) << format("Was able to send banner!\n", self->state.connection);

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
          string user_read = comm.read_from_user(bytes_read);

          if (bytes_read == 0) {
            if (self->state.break_count == 0) {
              aout(self) << "detected break\n";
              self->state.break_count++;
              self->send(self, WaitForInput_v);
            } else {
              aout(self) << "detected second break\n";
              comm.emit_line();
              aout(self) << format("Connection {} quit\n", connection);
              close(self->state.connection);
              self->state.connection = -1;
              self->system().registry().erase(self->state.registery_id);
              self->quit();
            }
          } else {
            if (user_read.find("\n") == string::npos) {
              self->state.current_input += user_read;
              self->send(self, WaitForInput_v);
            } else {
              string temp = self->state.current_input + user_read;
              temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
              string response = format("You said {}", temp);

              comm.emit_line(response);

              self->state.current_input.clear();
              self->send(self, PromptUser_v);
            }
          }
        } else {
          this_thread::sleep_for(chrono::milliseconds(200));
          self->send(self, WaitForInput_v);
        }
      },
      [=](CloseConnection) {
        aout(self) << format("Connection terminating: {}", self->state.connection);
        if (connection != -1)
          close(connection);
        self->state.connection = -1;
        self->quit();
      },
    };
  }

} // namespace primordia::mud