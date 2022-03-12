#pragma once

#include <string>
#include "yaml-cpp/yaml.h"
#include "fmt/format.h"

namespace primordia::mud::common {

  inline void yaml_to_buffer(fmt::memory_buffer& out, const YAML::Node node, const std::string& indent = "") {
    switch (node.Type()) {
    case YAML::NodeType::Null:
      break;
    case YAML::NodeType::Scalar:
      fmt::format_to(back_inserter(out), "{}{}", indent, node.as<string>());
      break;
    case YAML::NodeType::Sequence:
      fmt::format_to(back_inserter(out), "{}(seq)\n", indent);
      for (const auto& entry : node) {
        if (entry.IsScalar()) {
          yaml_to_buffer(out, entry, indent);
          fmt::format_to(back_inserter(out), "\n");
        } else {
          yaml_to_buffer(out, entry, indent);
        }
      }
      break;
    case YAML::NodeType::Map:
      fmt::format_to(back_inserter(out), "{}(map)\n", indent);
      for (const auto& entry : node) {
        fmt::format_to(back_inserter(out), "{}{}: ", indent, entry.first.as<string>());
        if (entry.second.IsScalar()) {
          yaml_to_buffer(out, entry.second);
          fmt::format_to(back_inserter(out), "\n");
        } else {
          fmt::format_to(back_inserter(out), "\n");
          yaml_to_buffer(out, entry.second, indent + "  ");
        }
      }
      break;
    case YAML::NodeType::Undefined:
      fmt::format_to(back_inserter(out), "(undef)\n");
      break;
    }
  }

} // namespace primordia::mud::common