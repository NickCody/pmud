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

  class CommStatic {

  public:
    CommStatic(int connection)
        : _connection(connection) {
      if (connection <= 0) {
        throw std::runtime_error(fmt::format("Connection {} is invalid!", connection));
      }
    }

    bool emit(const std::string& emission) {

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

    bool emit_line(const std::string& emission = "") {
      return emit(emission) && emit(NEWLINE);
    }

    bool emit(const std::vector<std::string>& emission) {
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

    std::string read_from_user(ssize_t& num_read) {
      // Read from the connection
      uint8_t buffer[MAX_READ];
      memset(buffer, 0, MAX_READ);
      num_read = read(_connection, buffer, MAX_READ - 1);

      if (buffer[0] == (uint8_t)0xff || (num_read == 1 && buffer[0] == 4)) {
        num_read = 0;
        return std::string();
      }

      std::string user_read((char*)buffer);
      user_read.erase(std::remove(user_read.begin(), user_read.end(), '\n'), user_read.end());
      return user_read;
    }

  private:
    inline static const ssize_t MAX_READ = 4096;
    inline static const std::string NEWLINE = "\n";
    inline static const std::string PROMPT = "pmud> ";

    inline static const std::vector<std::string> BANNER = { R"(                           _ )",
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