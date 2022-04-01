#pragma once

#include <string>

namespace primordia::mud::common {
  using namespace std;

  string replace_all(const string& text, const string& from, const string& to) {
    string replaced = text;
    for (auto at = replaced.find(from, 0); at != std::string::npos; at = replaced.find(from, at + to.length())) {
      replaced.replace(at, from.length(), to);
    }
    return replaced;
  }
} // namespace primordia::mud::common