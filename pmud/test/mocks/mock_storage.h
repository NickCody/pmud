#pragma once

#include <string>
#include <map>
#include <vector>
#include <set>

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

    optional<string> value_get(const string& key) override {
      if (!m_kv.contains(key))
        return nullopt;

      return m_kv[key];
    }

    bool map_store(const string& map_name, const string& key, const string& value) override {
      if (!m_map.contains(map_name)) {
        m_map[map_name] = {};
      }
      m_map[map_name][key] = value;
      return true;
    }

    optional<kv_t> map_get(const string map_name) override {
      if (!m_map.contains(map_name))
        return nullopt;

      return m_map[map_name];
    }

    bool list_store(const string& list_name, const string& value) override {
      if (!m_list.contains(list_name)) {
        m_list[list_name] = {};
      }
      m_list[list_name].push_back(value);
      return true;
    }

    optional<list_t> list_get(const string& list_name) override {
      if (!m_list.contains(list_name))
        return nullopt;

      return m_list[list_name];
    }

    bool set_store(const string& set_name, const string& value) override {
      if (!m_set.contains(set_name)) {
        m_set[set_name] = {};
      }
      m_set[set_name].insert(value);
      return true;
    }

    optional<set_t> set_get(const string& set_name) override {
      if (!m_set.contains(set_name))
        return nullopt;

      return m_set[set_name];
    }

    bool del_key(const string& key) override {
      m_kv.erase(key);
      return true;
    }

    bool event_store(const string& /*event_name*/, const StreamRecordFields_t& /*fields*/) override {
      return true;
    }

    bool stream_store(const string& /*map_name*/, const StreamRecordFields_t& /*fields*/) override {
      return true;
    }

    vector<StreamResponse> read_stream_raw(const string& /*command*/) override {
      return {};
    }

    vector<StreamResponse> read_stream_block(const string& /*stream_name*/, const string& /*pos*/, uint32_t /*timeout*/) override {
      return {};
    }

  private:
    kv_t m_kv;
    named_map_t m_map;
    named_list_t m_list;
    named_set_t m_set;
  };

} // namespace primordia::mud::test::mocks