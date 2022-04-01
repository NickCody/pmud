#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/core.h>

#include "caf/stateful_actor.hpp"
#include "common/util.h"
#include <spdlog/spdlog.h>
#include "controllers/command_actor.h"
#include "system/pmud_system.h"
#include "pnet/comm_static.h"

namespace primordia::mud::pnet {

  using namespace fmt;
  using namespace caf;
  using namespace std;
  using namespace primordia::mud::system;
  using namespace primordia::mud::controllers;

  // using namespace player;

  class ConnectionActor : public event_based_actor {
  public:
    ConnectionActor(actor_config& cfg, MudSystemPtr mud, const string& welcome, int connection)
        : event_based_actor(cfg),
          m_break_count(0),
          m_connection(connection),
          m_current_input(""),
          m_mud(mud) {

      m_command = actor_cast<strong_actor_ptr>(spawn<CommandActor>(mud, actor_cast<strong_actor_ptr>(this), m_connection));
      m_registery_id = format("Connection({})", id());
      system().registry().put(m_registery_id, this);

      attach_functor([this](const error& reason) {
        spdlog::info("Connection actor ({}) exiting, reason={}", m_connection, to_string(reason));
        send_exit(actor_cast<actor>(m_command), exit_reason::user_shutdown);
        m_command.reset();
      });

      CommStatic comm(m_connection);
      bool success = comm.emit_banner() && comm.emit_line() && comm.emit_line(welcome) && comm.emit_line() && comm.emit_line();
      if (!success) {
        spdlog::info("Failed to send welcome to connection {}", m_connection);
        // send(self, GoodbyeConnection());
      }
    }

    behavior make_behavior() {

      return {
        [this](PerformWelcome) {
          spdlog::debug("ConnectionActor::PerformWelcome({})", m_connection);
          send(actor_cast<actor>(m_command), PerformWelcome());
        },
        [this](ToUserEmit, string emission) {
          spdlog::debug("ConnectionActor::ToUserEmit({}) \"{}\" ", m_connection, emission);
          CommStatic(m_connection).emit_line(emission);
        },
        [this](ToUserPrompt, string prompt) {
          spdlog::debug("ConnectionActor::ToUserPrompt({}) \"{}\" ", m_connection, prompt);
          CommStatic(m_connection).emit_prompt(prompt);
          send(this, FromUserGetInput());
        },
        [this](FromUserGetInput) {
          CommStatic comm(m_connection);
          if (comm.has_data()) {
            ssize_t bytes_read = 0;
            string user_read = comm.read_from_user(bytes_read);
            spdlog::debug("ConnectionActor::FromUserGetInput({}) has_data \"{}\"", m_connection, user_read);

            if (bytes_read == 0) {
              if (m_break_count == 0) {
                // comm.emit_line("Detected quit, type again to quit!"); // todo: only do on virgin connection
                m_break_count++;
                send(this, FromUserGetInput());
              } else {
                spdlog::info("Connection {} quit", m_connection);
                send(this, GoodbyeConnection());
              }
            } else {
              m_break_count = 0;

              if (user_read.find("\n") == string::npos) {
                m_current_input += user_read;
                send(this, FromUserGetInput());
              } else {
                string final_user_read = comm.sanitize(m_current_input + user_read);
                m_current_input.clear();

                if (final_user_read == "quit" || final_user_read == "exit") {
                  send(this, GoodbyeConnection());
                } else {
                  // if (final_user_read.size() == 0)
                  //   comm.emit_line();

                  send(actor_cast<actor>(m_command), OnUserInput(), final_user_read);
                  send(this, FromUserGetInput());
                }
              }
            }
          } else {
            send(this, FromUserGetInput());
          }
        },
        [this](GoodbyeConnection) {
          spdlog::debug("ConnectionActor::GoodbyeConnection({})", m_connection);
          if (m_connection != -1) {
            CommStatic comm(m_connection);
            comm.emit_line();
            comm.emit_line("Goodbye!");

            // ZMQ:
            //
            close(m_connection);
          }
          system().registry().erase(m_registery_id);

          send_exit(actor_cast<actor>(this), exit_reason::user_shutdown);
        },
      };
    }

  private:
    int m_break_count;
    strong_actor_ptr m_command;
    int m_connection;
    string m_current_input;
    MudSystemPtr m_mud;
    string m_registery_id;
  };

} // namespace primordia::mud::pnet