#include <fmt/format.h>
#include <initializer_list>
#include <math.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <ranges>
#include <list>
#include <type_traits>

#include <cnl/all.h>

using namespace std;

using cnl::power;
using cnl::scaled_integer;
using cnl::fixed_point;

using ScaledInt64 = scaled_integer<std::int64_t, power<-5, 10>>;

auto main() -> int {

  auto ints = std::initializer_list<ScaledInt64>{ 0, 1, 2, 3, 4, 5 };
  auto even = [](ScaledInt64 i) { return 0 == static_cast<int>(i) % 2; };
  auto square = [](ScaledInt64 i) { return i * i; };
  auto div3 = [](ScaledInt64 i) { return i / 3; };

  // "pipe" syntax of composing the views:
  for (ScaledInt64 i : ints | views::filter(even) | views::transform(square) | views::transform(div3)) {
    std::cout << i << ' ';
  }

  std::cout << '\n';

  // a traditional "functional" composing syntax:
  for (ScaledInt64 i : views::transform(views::filter(ints, even), square)) {
    std::cout << i << ' ';
  }

  std::cout << '\n';

  auto result = ScaledInt64{10} / 3;
  std::cout << result << '\n';

  std::cout << '\n';
}