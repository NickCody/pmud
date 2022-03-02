
#include <fstream>
#include <iostream>
#include <memory>
#include "fmt/core.h"

namespace primordia::mud::common {

  using namespace std;

  struct StreamDelete {
    void operator()(istream* is_ptr) const {
      if (is_ptr && is_ptr != &cin) {
        static_cast<ifstream*>(is_ptr)->close();
        delete is_ptr;
      }
    }
  };

  inline unique_ptr<istream, StreamDelete> stdout_or_file(const char* filename) {

    StreamDelete deletor;
    unique_ptr<istream, StreamDelete> is_ptr{ nullptr, deletor };

    if (filename == nullptr || strlen(filename) == 0) {
      fmt::print(stderr, "Reading YAML from stdin...\n");
      is_ptr.reset(&cin);
    } else {
      is_ptr.reset(new ifstream(filename));
    }

    return is_ptr;
  }

} // namespace primordia::mud::common