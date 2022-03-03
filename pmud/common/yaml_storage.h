#pragma once

#include <string>
#include <algorithm>
#include "yaml-cpp/yaml.h"
#include "storage/storage.h"

namespace primordia::mud::common {

  using namespace std;
  using namespace primordia::mud::storage;

  inline void yaml_to_storage(Storage* storage, const string& path, const YAML::Node node) {

    switch (node.Type()) {
    case YAML::NodeType::Null:
      break;

    case YAML::NodeType::Scalar:
      storage->value_store(path, node.as<string>());
      break;

    case YAML::NodeType::Sequence:
      for (const auto& entry : node) {
        if (entry.IsScalar()) {
          storage->list_store(path, entry.as<string>());
        } else {
          yaml_to_storage(storage, path, entry);
        }
      }
      break;

    case YAML::NodeType::Map: {

      for (const auto& entry : node) {

        if (entry.second.IsScalar()) {
          auto p = path + ":" + node["name"].as<string>();
          storage->map_store(p, entry.first.as<string>(), entry.second.as<string>());
        } else {
          auto p = path + ":" + entry.first.as<string>();
          yaml_to_storage(storage, p, entry.second);
        }
      }
      break;
    }
    case YAML::NodeType::Undefined:
      break;
    }
  }

} // namespace primordia::mud::common