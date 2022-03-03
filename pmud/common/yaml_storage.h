#pragma once

#include <string>
#include <algorithm>
#include "yaml-cpp/yaml.h"
#include "storage/storage.h"

namespace primordia::mud::common {

  using namespace std;
  using namespace primordia::mud::storage;

  enum DataStructure { String, List, Set, Hash, SortedSet };

  inline void yaml_to_storage(Storage* storage, const string& path, const YAML::Node node, DataStructure ds = DataStructure::String) {

    switch (node.Type()) {
    case YAML::NodeType::Null:
      break;

    case YAML::NodeType::Scalar:
      switch (ds) {
      case DataStructure::String:
        storage->value_store(path, node.as<string>());
        break;
      case DataStructure::List:
        storage->list_store(path, node.as<string>());
        break;
      default:
        fmt::print("Skipping {} as data structure was unhandled.\n", node.as<string>());
      }
      break;

    case YAML::NodeType::Sequence:
      for (const auto& entry : node) {
        yaml_to_storage(storage, path, entry, DataStructure::List);
      }
      break;

    case YAML::NodeType::Map: {

      for (const auto& entry : node) {
        if (entry.second.IsScalar()) {
          auto p = path + ":" + node["name"].as<string>();
          storage->map_store(p, entry.first.as<string>(), entry.second.as<string>());
        } else if (entry.second.IsSequence()) {
          yaml_to_storage(storage, path + ":" + entry.first.as<string>(), entry.second, DataStructure::List);
        } else if (entry.second.IsMap()) {
          yaml_to_storage(storage, path + ":" + entry.first.as<string>(), entry.second, DataStructure::Hash);
        } else {
          fmt::print("Skipping {}:{} as data structure was unhandled.\n", entry.first.as<string>(), entry.second.as<string>());
        }
      }
      break;
    }
    case YAML::NodeType::Undefined:
      break;
    }
  }

} // namespace primordia::mud::common