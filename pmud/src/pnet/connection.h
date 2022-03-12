#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/core.h>

#include "caf/stateful_actor.hpp"
#include "server_state.h"
#include "util.h"
#include "spdlog/spdlog.h"
#include "command.h"
#include "system/pmud_system.h"

namespace primordia::mud {

  using namespace fmt;
  using namespace caf;
  using namespace std;
  using namespace system;

  // using namespace player;

  class Connection : public event_based_actor {
  public:
    Connection(actor_config& cfg, MudSystemPtr mud, const string welcome, int connection)
        : event_based_actor(cfg),
          m_welcome(welcome),
          m_mud(mud) {

      state.connection = connection;
      state.break_count = 0;
      state.current_input = "";
      state.command = actor_cast<strong_actor_ptr>(spawn<Command>(mud, actor_cast<strong_actor_ptr>(this), connection));
      state.registery_id = format("Connection({})", id());
      system().registry().put(state.registery_id, this);

      attach_functor([this](const error& /*reason*/) {
        SPDLOG_INFO("Connection actor exiting...", state.connection);
        send_exit(actor_cast<actor>(state.command), exit_reason::user_shutdown);
        state.command.reset();
      });

      {
        CommStatic comm(state.connection);
        bool success = comm.emit_banner() && comm.emit_line() && comm.emit_line(welcome) && comm.emit_line() && comm.emit_line();
        if (!success) {
          SPDLOG_INFO("Failed to send welcome to connection {}", state.connection);
          // send(self, GoodbyeConnection_v);
        }
      }
    }

    behavior make_behavior() {
      return {
        [this](PerformWelcome) { send(actor_cast<actor>(state.command), PerformWelcome_v); },
        [this](ToUserEmit, string emission) { CommStatic(state.connection).emit_line(emission); },
        [this](ToUserPrompt, string prompt) {
          CommStatic(state.connection).emit_prompt(prompt);
          send(this, FromUserGetInput_v);
        },
        [this](FromUserGetInput) {
          CommStatic comm(state.connection);
          if (comm.has_data()) {
            ssize_t bytes_read = 0;
            string user_read = comm.read_from_user(bytes_read);

            if (bytes_read == 0) {
              if (state.break_count == 0) {
                // comm.emit_line("Detected quit, type again to quit!"); // todo: only do on virgin connection
                state.break_count++;
                send(this, FromUserGetInput_v);
              } else {
                SPDLOG_INFO("Connection {} quit", state.connection);
                send(this, GoodbyeConnection_v);
              }
            } else {
              state.break_count = 0;

              if (user_read.find("\n") == string::npos) {
                state.current_input += user_read;
                send(this, FromUserGetInput_v);
              } else {
                string final_user_read = comm.sanitize(state.current_input + user_read);
                state.current_input.clear();

                if (final_user_read == "quit" || final_user_read == "exit") {
                  send(this, GoodbyeConnection_v);
                } else {
                  // if (final_user_read.size() == 0)
                  //   comm.emit_line();

                  send(actor_cast<actor>(state.command), OnUserInput_v, final_user_read);
                  send(this, FromUserGetInput_v);
                }
              }
            }
          } else {
            send(this, FromUserGetInput_v);
          }
        },
        [this](GoodbyeConnection) {
          if (state.connection != -1) {
            CommStatic comm(state.connection);
            comm.emit_line();
            comm.emit_line("Goodbye!");
            close(state.connection);
          }
          system().registry().erase(state.registery_id);

          send_exit(actor_cast<actor>(this), exit_reason::user_shutdown);
        },
      };
    }

  private:
    const string& m_welcome;
    ConnectionState state;
    MudSystemPtr m_mud;
  };

} // namespace primordia::mud