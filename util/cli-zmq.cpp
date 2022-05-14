#include <string>
#include <fmt/format.h>
#include <zmq.hpp>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char** argv) {
  zmq::context_t context{ 1 };
  zmq::socket_t socket{ context, zmq::socket_type::stream };
  string connect("tcp://localhost:49201");

  if (argc > 1) {
    connect = argv[1];
  }

  fmt::print("Using connection: {}", connect.c_str());
  socket.connect(connect.c_str());

  while (true) {
    zmq::message_t reply{};
    zmq::recv_result_t result = socket.recv(reply, zmq::recv_flags::none);
    if (result.has_value()) {
      fmt::print("{}\n", result.value());
      string input;
      cin >> input;
      socket.send(zmq::buffer(input), zmq::send_flags::none);
    } else {
      fmt::print("Nothing from server\n");
    }
  }

  return 0;
}