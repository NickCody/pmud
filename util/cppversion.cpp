#include <iostream>

#if __cplusplus <= 199711L
static const char* cpp_version = "C++1997";
#elif __cplusplus <= 201103L
static const char* cpp_version = "C++11";
#elif __cplusplus <= 201402L
static const char* cpp_version = "C++14";
#elif __cplusplus <= 201703L
static const char* cpp_version = "C++17";
#elif __cplusplus <= 202002L
static const char* cpp_version = "C++20";
#else
static const char* cpp_version = "C++Unknown";
#endif

#include "include/fmt/core.h"

int main() {
  std::cout << fmt::format("{}", cpp_version) << std::endl;
}