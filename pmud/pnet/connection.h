#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/format.h>

#include "caf/stateful_actor.hpp"
#include "server_state.h"
#include "util.h"
#include "logger/logger.h"
#include "command.h"

// #include "player/login_coordinator.h"

namespace primordia::mud {

  using namespace fmt;
  using namespace caf;
  using namespace std;

  // using namespace player;

  behavior Connection(stateful_actor<ConnectionState>* self, const string welcome, int connection) {
    self->state.connection = connection;
    self->state.break_count = 0;
    self->state.current_input = "";
    self->state.command = actor_cast<strong_actor_ptr>(self->spawn<Command>(actor_cast<strong_actor_ptr>(self), connection));
    self->state.registery_id = format("Connection({})", self->id());
    self->system().registry().put(self->state.registery_id, self);

    self->attach_functor([=](const error& /*reason*/) { self->state.command.reset(); });

    {
      CommStatic comm(self->state.connection);
      bool success = comm.emit_banner() && comm.emit_line() && comm.emit_line(welcome) && comm.emit_line() && comm.emit_line();
      if (!success) {
        LOG_INFO("Failed to send welcome to connection {}", self->state.connection);
        // self->send(self, GoodbyeConnection_v);
      }
    }

    LOG_INFO("Was able to send banner!", self->state.connection);

    return {
      [=](PerformWelcome) { self->send(actor_cast<actor>(self->state.command), PerformWelcome_v); },
      [=](ToUserEmit, string emission) { CommStatic(self->state.connection).emit_line(emission); },
      [=](ToUserPrompt, string prompt) {
        CommStatic(self->state.connection).emit_prompt(prompt);
        self->send(self, FromUserGetInput_v);
      },
      [=](FromUserGetInput) {
        CommStatic comm(self->state.connection);
        if (comm.has_data()) {
          ssize_t bytes_read = 0;
          string user_read = comm.read_from_user(bytes_read);

          if (bytes_read == 0) {
            if (self->state.break_count == 0) {
              // comm.emit_line("Detected quit, type again to quit!"); // todo: only do on virgin connection
              self->state.break_count++;
              self->send(self, FromUserGetInput_v);
            } else {
              LOG_INFO("Connection {} quit", connection);
              self->send(self, GoodbyeConnection_v);
            }
          } else {
            self->state.break_count = 0;

            if (user_read.find("\n") == string::npos) {
              self->state.current_input += user_read;
              self->send(self, FromUserGetInput_v);
            } else {
              string final_user_read = comm.sanitize(self->state.current_input + user_read);
              self->state.current_input.clear();

              if (final_user_read == "quit" || final_user_read == "exit") {
                self->send(self, GoodbyeConnection_v);
              } else {
                // if (final_user_read.size() == 0)
                //   comm.emit_line();

                self->send(actor_cast<actor>(self->state.command), OnUserInput_v, final_user_read);
                self->send(self, FromUserGetInput_v);
              }
            }
          }
        } else {
          this_thread::sleep_for(chrono::milliseconds(200));
          self->send(self, FromUserGetInput_v);
        }
      },
      [=](GoodbyeConnection) {
        LOG_INFO("Connection terminating: {}", self->state.connection);
        if (connection != -1) {
          CommStatic comm(self->state.connection);
          comm.emit_line("Goodbye!");
          close(connection);
        }
        self->system().registry().erase(self->state.registery_id);

        self->send_exit(actor_cast<actor>(self->state.command), exit_reason::user_shutdown);
        self->send_exit(actor_cast<actor>(self), exit_reason::user_shutdown);
      },
    };
  }

} // namespace primordia::mud