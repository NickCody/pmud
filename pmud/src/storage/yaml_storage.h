#pragma once

#include <regex>
#include <string>
#include <algorithm>
#include "yaml-cpp/yaml.h"
#include "storage/storage.h"

namespace primordia::mud::storage {

  using namespace std;

  inline void yaml_to_storage(Storage* storage, const string& path, const YAML::Node node, bool idempotence = true) {

    switch (node.Type()) {
    case YAML::NodeType::Null:
      break;

    case YAML::NodeType::Scalar:
      storage->value_store(path, node.as<string>());
      break;

    case YAML::NodeType::Sequence:
      for (const auto& entry : node) {
        if (entry.IsScalar()) {
          storage->set_store(path, entry.as<string>());
        } else {
          yaml_to_storage(storage, path, entry);
        }
      }
      break;

    case YAML::NodeType::Map: {
      bool virgin = true;
      for (const auto& entry : node) {
        if (entry.second.IsScalar()) {
          storage->map_store(path, entry.first.as<string>(), entry.second.as<string>());
        } else {
          auto new_path = path + ":" + std::regex_replace(entry.first.as<string>(), std::regex("_"), ":");

          if (idempotence && virgin) {
            storage->del_key(new_path);
          }
          yaml_to_storage(storage, new_path, entry.second);
        }
      }
      break;
    }
    case YAML::NodeType::Undefined:
      break;
    }
  }

} // namespace primordia::mud::storage