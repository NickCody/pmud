#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/core.h>

#include "caf/stateful_actor.hpp"
#include "util.h"
#include <spdlog/spdlog.h>
#include "command_actor.h"
#include "system/pmud_system.h"
#include "pnet/comm_static.h"

namespace primordia::mud::pnet {

  using namespace fmt;
  using namespace caf;
  using namespace std;
  using namespace primordia::mud::system;

  // using namespace player;

  class ConnectionActor : public event_based_actor {
  public:
    ConnectionActor(actor_config& cfg, MudSystemPtr mud, const string welcome, int _connection)
        : event_based_actor(cfg),
          m_welcome(welcome),
          m_mud(mud) {

      connection = _connection;
      break_count = 0;
      current_input = "";
      command = actor_cast<strong_actor_ptr>(spawn<CommandActor>(mud, actor_cast<strong_actor_ptr>(this), connection));
      registery_id = format("Connection({})", id());
      system().registry().put(registery_id, this);

      attach_functor([this](const error& /*reason*/) {
        SPDLOG_INFO("Connection actor exiting...", this->connection);
        send_exit(actor_cast<actor>(command), exit_reason::user_shutdown);
        command.reset();
      });

      {
        CommStatic comm(connection);
        bool success = comm.emit_banner() && comm.emit_line() && comm.emit_line(welcome) && comm.emit_line() && comm.emit_line();
        if (!success) {
          SPDLOG_INFO("Failed to send welcome to connection {}", connection);
          // send(self, GoodbyeConnection());
        }
      }
    }

    behavior make_behavior() {
      return {
        [this](PerformWelcome) {
          SPDLOG_DEBUG("ConnectionActor::PerformWelcome({})", connection);
          send(actor_cast<actor>(command), PerformWelcome());
        },
        [this](ToUserEmit, string emission) {
          SPDLOG_DEBUG("ConnectionActor::ToUserEmit({}) \"{}\" ", connection, emission);
          CommStatic(connection).emit_line(emission);
        },
        [this](ToUserPrompt, string prompt) {
          SPDLOG_DEBUG("ConnectionActor::ToUserPrompt({}) \"{}\" ", connection, prompt);
          CommStatic(connection).emit_prompt(prompt);
          send(this, FromUserGetInput());
        },
        [this](FromUserGetInput) {
          SPDLOG_DEBUG("ConnectionActor::FromUserGetInput({})", connection);
          CommStatic comm(connection);
          if (comm.has_data()) {
            ssize_t bytes_read = 0;
            string user_read = comm.read_from_user(bytes_read);
            SPDLOG_DEBUG("ConnectionActor::FromUserGetInput({}) has_data \"{}\"", connection, user_read);

            if (bytes_read == 0) {
              if (break_count == 0) {
                // comm.emit_line("Detected quit, type again to quit!"); // todo: only do on virgin connection
                break_count++;
                send(this, FromUserGetInput());
              } else {
                SPDLOG_INFO("Connection {} quit", connection);
                send(this, GoodbyeConnection());
              }
            } else {
              break_count = 0;

              if (user_read.find("\n") == string::npos) {
                current_input += user_read;
                send(this, FromUserGetInput());
              } else {
                string final_user_read = comm.sanitize(current_input + user_read);
                current_input.clear();

                if (final_user_read == "quit" || final_user_read == "exit") {
                  send(this, GoodbyeConnection());
                } else {
                  // if (final_user_read.size() == 0)
                  //   comm.emit_line();

                  send(actor_cast<actor>(command), OnUserInput(), final_user_read);
                  send(this, FromUserGetInput());
                }
              }
            }
          } else {
            send(this, FromUserGetInput());
          }
        },
        [this](GoodbyeConnection) {
          SPDLOG_DEBUG("ConnectionActor::GoodbyeConnection({})", connection);
          if (connection != -1) {
            CommStatic comm(connection);
            comm.emit_line();
            comm.emit_line("Goodbye!");

            // ZMQ:
            //
            close(connection);
          }
          system().registry().erase(registery_id);

          send_exit(actor_cast<actor>(this), exit_reason::user_shutdown);
        },
      };
    }

  private:
    const string& m_welcome;
    int connection;
    std::string registery_id;
    int break_count;
    std::string current_input;
    strong_actor_ptr command;
    MudSystemPtr m_mud;
  };

} // namespace primordia::mud::pnet