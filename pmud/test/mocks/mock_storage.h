#pragma once

#include <string>
#include <map>
#include <vector>
#include <set>

#include "storage/storage.h"

namespace primordia::mud::test::mock {

  using namespace std;
  using namespace primordia::mud::storage;

  class MockStorage : public Storage {
    using kv_t = map<string, string>;
    using named_map_t = map<string, kv_t>;
    using named_list_t = map<string, vector<string>>;
    using named_set_t = map<string, set<string>>;

  public:
    bool init() override {
      return true;
    }

    bool value_store(const string& key, const string& value) override {
      m_kv[key] = value;
      return true;
    }

    bool map_store(const string& map_name, const string& key, const string& value) override {
      if (!m_map.contains(map_name)) {
        m_map[map_name] = {};
      }
      m_map[map_name][key] = value;
      return true;
    }

    bool list_store(const string& list_name, const string& value) override {
      if (!m_list.contains(list_name)) {
        m_list[list_name] = {};
      }
      m_list[list_name].push_back(value);
      return true;
    }

    bool set_store(const string& set_name, const string& value) override {
      if (!m_set.contains(set_name)) {
        m_set[set_name] = {};
      }
      m_set[set_name].insert(value);
      return true;
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

} // namespace primordia::mud::test::mock