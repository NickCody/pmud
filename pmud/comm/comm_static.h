#pragma once

#include <sys/socket.h>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fmt/format.h>

namespace primordia::mud {

  using namespace fmt;
  using namespace std;

  class CommStatic {

  public:
    CommStatic(int connection)
        : _connection(connection) {
      if (connection <= 0) {
        throw runtime_error(fmt::format("Connection {} is invalid!", connection));
      }
    }

    bool emit(const string& emission) {

      auto bytes_sent = send(_connection, emission.c_str(), emission.size(), 0);

      if (bytes_sent == -1)
        return false;

      return true;
    }

    bool emit_prompt() {
      return emit(PROMPT);
    }

    bool emit_banner() {
      return emit(BANNER);
    }

    bool emit_line(const string& emission = "") {
      return emit(emission) && emit(CR) && emit(NEWLINE);
    }

    bool emit(const vector<string>& emission) {
      for (auto& line : emission) {
        if (!emit_line(line))
          return false;
      }
      return true;
    }

    bool has_data() {
      // Read from the connection
      uint8_t buffer[MAX_READ];
      memset(buffer, 0, MAX_READ);
      return recv(_connection, buffer, MAX_READ - 1, MSG_PEEK) > 0;
    }

    string read_from_user(ssize_t& num_read) {
      // Read from the connection
      uint8_t buffer[MAX_READ];
      memset(buffer, 0, MAX_READ);
      num_read = read(_connection, buffer, MAX_READ - 1);

      if (buffer[0] == (uint8_t)0xff || (num_read == 1 && buffer[0] == 4)) {
        num_read = 0;
        return string();
      }

      return string((char*)buffer);
    }

    string sanitize(const string& input) {
      string final;
      for (auto c : input) {
        if (c >= 32 && c <= 95) {
          final += c;
        }
      }
      return final;
    }

  private:
    inline static const ssize_t MAX_READ = 4096;
    inline static const string NEWLINE = "\n";
    inline static const string CR = format("{}", (char)0x0D);
    inline static const string PROMPT = "pmud> ";

    inline static const vector<string> BANNER = { R"(                           _ )",
                                                  R"(                          | |)",
                                                  R"( _ __  _ __ ___  _   _  __| |)",
                                                  R"(| '_ \| '_ ` _ \| | | |/ _` |)",
                                                  R"(| |_) | | | | | | |_| | (_| |)",
                                                  R"(| .__/|_| |_| |_|\__,_|\__,_|)",
                                                  R"(| |)",
                                                  R"(|_|)" };

  private:
    int _connection;
  };

} // namespace primordia::mud