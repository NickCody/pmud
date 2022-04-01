/**
 *  pmud_io.h
 *
 *  Created on Thu Mar 24 2022
 */

#include <fstream>
#include <iostream>
#include <memory>
#include <fmt/core.h>

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

  /**
   * \brief Returns a file input stream or stdin if no file specified.
   *
   * @param filename Optional filename for which to open and create input stream
   * @return The file input stream
   *
   */

  inline unique_ptr<istream, StreamDelete> stdout_or_file(const char* filename) {

    StreamDelete deletor;
    unique_ptr<istream, StreamDelete> is_ptr{ nullptr, deletor };

    if (filename == nullptr || strlen(filename) == 0) {
      is_ptr.reset(&cin);
    } else {
      auto ifs = new ifstream(filename);
      if (ifs->is_open())
        is_ptr.reset(ifs);
      else
        is_ptr.release();
    }

    return is_ptr;
  }

} // namespace primordia::mud::common