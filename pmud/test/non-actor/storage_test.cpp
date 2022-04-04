#include <optional>
#include <string>
#include <gtest/gtest.h>
#include <fmt/format.h>

#include "yaml-cpp/yaml.h"
#include "storage/storage.h"
#include "mocks/mock_storage.h"

#include "storage/yaml_storage.h"

using namespace std;
using namespace primordia::mud::test::mocks;
using namespace primordia::mud::storage;

class StorageTestFixture : public ::testing::Test {
protected:
  unique_ptr<Storage> storage = std::make_unique<MockStorage>();
};

TEST_F(StorageTestFixture, SequenceTest) {
  string yaml_source = R"""(monster-kind:
  - undead
  - feral
  - magical
  - demonic
  - celestial)""";

  YAML::Node yaml = YAML::Load(yaml_source);
  yaml_to_storage(storage.get(), "test", yaml);

  auto set_opt = storage->set_get("test:monster-kind");
  ASSERT_NE(set_opt, nullopt);
  ASSERT_EQ(set_opt.value().size(), 5);
}

TEST_F(StorageTestFixture, MapTest) {
  string yaml_source = R"""(monster_abominable snowman:
  kind: feral
  hit-dice: "4d6"
monster_banshee:
  kind: magical
  hit-dice: "4d10")""";

  YAML::Node yaml = YAML::Load(yaml_source);
  yaml_to_storage(storage.get(), "monsters", yaml);

  {
    auto temp = storage->map_get("monsters:monster:abominable snowman");
    ASSERT_NE(temp, nullopt);
    ASSERT_EQ(temp.value().size(), 2);
  }

  {
    auto temp = storage->map_get("monsters:monster:banshee");
    ASSERT_NE(temp, nullopt);
    ASSERT_EQ(temp.value().size(), 2);
  }
}
