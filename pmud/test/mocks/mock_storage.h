#pragma once

#include <string>
#include <map>
#include <vector>
#include <set>

#include "pmud/src/common/storage_types.h"
#include "storage/storage.h"

namespace primordia::mud::test::mocks {

  using namespace std;
  using namespace primordia::mud::storage;

  class MockStorage : public Storage {

  public:
    bool init() override {
      return true;
    }

    bool value_store(const string& key, const string& value) override {
      m_kv[key] = value;
      return true;
    }

    common::opt_string_t value_get(const string& key) const override {
      if (!m_kv.contains(key))
        return nullopt;

      return m_kv.at(key);
    }

    bool map_store(const string& map_name, const string& key, const string& value) override {
      if (!m_map.contains(map_name)) {
        m_map[map_name] = {};
      }
      m_map[map_name][key] = value;
      return true;
    }

    common::opt_map_t map_get(const string map_name) const override {
      if (!m_map.contains(map_name))
        return nullopt;

      return m_map.at(map_name);
    }

    bool list_store(const string& list_name, const string& value) override {
      if (!m_list.contains(list_name)) {
        m_list[list_name] = {};
      }
      m_list[list_name].push_back(value);
      return true;
    }

    common::opt_list_t list_get(const string& list_name) const override {
      if (!m_list.contains(list_name))
        return nullopt;

      return m_list.at(list_name);
    }

    bool set_store(const string& set_name, const string& value) override {
      if (!m_set.contains(set_name)) {
        m_set[set_name] = {};
      }
      m_set[set_name].insert(value);
      return true;
    }

    common::opt_set_t set_get(const string& set_name) const override {
      if (!m_set.contains(set_name))
        return nullopt;

      return m_set.at(set_name);
    }

    bool del_key(const string& key) override {
      if (m_kv.contains(key))
        m_kv.erase(key);
      else if (m_map.contains(key))
        m_map.erase(key);
      else if (m_list.contains(key))
        m_list.erase(key);
      else if (m_set.contains(key))
        m_set.erase(key);
      else
        return false;

      return true;
    }

    bool event_store(const string& /*event_name*/, const common::StreamRecordFields_t& /*fields*/) override {
      return true;
    }

    bool stream_store(const string& /*map_name*/, const common::StreamRecordFields_t& /*fields*/) override {
      return true;
    }

    vector<common::StreamResponse> read_stream_raw(const string& /*command*/) const override {
      return {};
    }

    vector<common::StreamResponse> read_stream_block(const string& /*stream_name*/, const string& /*pos*/, uint32_t /*timeout*/) const override {
      return {};
    }

  private:
    common::map_t m_kv;
    common::named_map_t m_map;
    common::named_list_t m_list;
    common::named_set_t m_set;
  };

} // namespace primordia::mud::test::mocks