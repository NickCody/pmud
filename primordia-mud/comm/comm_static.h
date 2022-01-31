#pragma once

#include <sys/socket.h>
#include <algorithm>
#include <string>
#include <thread>

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
    // fmt::print("emit: {}, sent {} bytes\n", emission, bytes_sent);

    if (bytes_sent == -1)
      return false;
    else
      return true;

    // int flags = fcntl(_connection, F_GETFL) & ~O_NONBLOCK;
    // fcntl(_connection, F_SETFL, flags);

    // ssize_t bytes_sent = 0;
    // while (bytes_sent < (ssize_t)emission.size()) {
    //   auto this_bytes_sent = send(_connection, emission.c_str(), emission.size(), 0);
    //   if (this_bytes_sent == -1) {
    //     if (errno == EWOULDBLOCK || errno == EAGAIN)
    //       continue;
    //   } else {
    //     return false;
    //   }
    //   bytes_sent += this_bytes_sent;
    // }
    // fcntl(_connection, F_SETFL, flags | O_NONBLOCK);

    return true;
    // while (true) {
    //   ssize_t sent_bytes = send(_connection, emission.c_str(), emission.size(), MSG_DONTWAIT);
    //   if (sent_bytes == -1) {
    //     if (errno == EWOULDBLOCK || errno == EAGAIN)
    //       continue;
    //     else
    //       return false;
    //   }
    //   return true;
    // }
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