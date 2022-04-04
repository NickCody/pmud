#pragma once

#include <arpa/inet.h>
#include <ctime>
#include <limits.h>
#include <map>
#include <memory>
#include <netdb.h>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>

#include "common/util.h"
#include "storage/storage_types.h"

namespace primordia::mud::storage {

  using namespace std;

  using kv_t = map<string, string>;
  using list_t = vector<string>;
  using set_t = set<string>;

  using named_map_t = map<string, kv_t>;
  using named_list_t = map<string, list_t>;
  using named_set_t = map<string, set_t>;

  class Storage {
  public:
    virtual ~Storage(){};
    virtual bool init() = 0;

    virtual bool value_store(const string& key, const string& value) = 0;
    virtual std::optional<string> value_get(const string& key) = 0;

    virtual bool map_store(const string& map_name, const string& key, const string& value) = 0;
    virtual std::optional<kv_t> map_get(const string map_name) = 0;

    virtual bool list_store(const string& list_name, const string& value) = 0;
    virtual std::optional<list_t> list_get(const string& list_name) = 0;

    virtual bool set_store(const string& set_name, const string& value) = 0;
    virtual std::optional<set_t> set_get(const string& set_name) = 0;

    virtual bool del_key(const string& key) = 0;
    virtual bool event_store(const string& event_name, const StreamRecordFields_t& fields) = 0;
    virtual bool stream_store(const string& map_name, const StreamRecordFields_t& fields) = 0;
    virtual vector<StreamResponse> read_stream_raw(const string& command) = 0;
    virtual vector<StreamResponse> read_stream_block(const string& stream_name, const string& pos, uint32_t timeout) = 0;
  };

} // namespace primordia::mud::storage
