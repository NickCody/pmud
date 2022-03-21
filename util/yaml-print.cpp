#include <string>
#include <unistd.h>
#include "common/pmud_io.h"
#include "common/yaml_util.h"
#include "fmt/format.h"

using namespace std;
using namespace primordia::mud::common;

void usage() {
  fmt::print("Usage: yaml-print [-f filename]\n\n");
}

int main(int argc, char** argv) {

  string filename;
  string prefix;

  int c;
  opterr = 0;
  while ((c = getopt(argc, argv, "f:p:h")) != -1) {
    switch (c) {
    case 'f':
      filename = optarg;
      break;
    case 'h':
      usage();
      return EXIT_SUCCESS;
    default:
      usage();
      return EXIT_FAILURE;
    }
  }

  auto input = stdout_or_file(filename.c_str());

  YAML::Node config = YAML::Load(*input);

  auto buffer = fmt::memory_buffer();
  yaml_to_buffer(buffer, config);
  fmt::print("{}", buffer.data());

  return EXIT_SUCCESS;
}