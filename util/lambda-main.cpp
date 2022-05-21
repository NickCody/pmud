#include <fmt/format.h>

auto main(int, char**, char** env) -> int {

  auto offset = 0uz;

  while (true) {
    if (env[offset] != nullptr) {
      fmt::print("{}\n", env[offset]);
      ++offset;
    } else {
      break;
    }
  }

  return 0;
}