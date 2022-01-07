#include "include/fmt/format.h"
#include "include/fmt/printf.h"

#include <memory>

int main(int /*argc*/, char** /*argv*/) {

  std::unique_ptr<int> pint = std::make_unique<int>();
  std::shared_ptr<int> spint = std::move(pint);
  *spint = 10;

  fmt::printf("Value is %d\n", *spint);

  return 0;
}