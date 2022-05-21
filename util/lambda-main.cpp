#include <fmt/format.h>

auto main(int, char**, char** env) -> int {

  auto offset = 0uz;

  while (env[offset]) {
    fmt::print("{}\n", env[offset++]);
  }

  return 0;
}