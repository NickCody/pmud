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
      if (emission.size() > 0) {
        auto bytes_sent = send(_connection, emission.c_str(), emission.size(), 0);

        if (bytes_sent == -1)
          return false;
      }
      return true;
    }

    bool emit_prompt(const string& prompt = DEFAULT_PROMPT) {
      return emit(fmt::format("{}> ", prompt));
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
      uint8_t buffer[BUFFER_SIZE];
      memset(buffer, 0, BUFFER_SIZE);
      return recv(_connection, buffer, BUFFER_MAX_READ, MSG_PEEK) > 0;
    }

    string read_from_user(ssize_t& num_read) {
      uint8_t buffer[BUFFER_SIZE];
      memset(buffer, 0, BUFFER_SIZE);
      num_read = read(_connection, buffer, BUFFER_MAX_READ);

      if (buffer[0] == 0xff || (num_read == 1 && buffer[0] == 4)) {
        num_read = 0;
        return string();
      }

      return lite_sanitize(buffer);
    }

    // preserves newline
    //
    string lite_sanitize(uint8_t* buffer) {
      stringstream cleaned;
      for (ssize_t i = 0; buffer[i] != 0 && i < BUFFER_MAX_READ; i++) {
        uint8_t c = buffer[i];
        if (c >= 10 && c <= 127) {
          cleaned << (char)c;
        }
      }
      return cleaned.str();
    }

    string sanitize(const string& input) {

      stringstream cleaned;
      for (auto c : input) {
        if ((uint8_t)c >= 32 && (uint8_t)c <= 127) {
          cleaned << c;
        }
      }
      return cleaned.str();
    }

  public:
    inline static const ssize_t BUFFER_MAX_READ = 512;
    inline static const ssize_t BUFFER_PADDING = 128;
    inline static const ssize_t BUFFER_SIZE = BUFFER_MAX_READ + BUFFER_PADDING;
    inline static const string NEWLINE = "\n";
    inline static const string CR = format("{}", (char)0x0D);
    inline static const string DEFAULT_PROMPT = "pmud";

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