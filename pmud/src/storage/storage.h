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
#include "common/storage_types.h"
#include "pmud/src/common/storage_types.h"

namespace primordia::mud::storage {

  using namespace std;

  class Storage {
  public:
    virtual ~Storage(){};
    virtual bool init() = 0;

    virtual bool value_store(const string& key, const string& value) = 0;
    virtual common::opt_string_t value_get(const string& key) const = 0;

    virtual bool map_store(const string& map_name, const string& key, const string& value) = 0;
    virtual common::opt_map_t map_get(const string map_name) const = 0;

    virtual bool list_store(const string& list_name, const string& value) = 0;
    virtual common::opt_list_t list_get(const string& list_name) const = 0;

    virtual bool set_store(const string& set_name, const string& value) = 0;
    virtual common::opt_set_t set_get(const string& set_name) const = 0;

    virtual bool del_key(const string& key) = 0;
    virtual bool event_store(const string& event_name, const common::StreamRecordFields_t& fields) = 0;
    virtual bool stream_store(const string& map_name, const common::StreamRecordFields_t& fields) = 0;
    virtual vector<common::StreamResponse> read_stream_raw(const string& command) const = 0;
    virtual vector<common::StreamResponse> read_stream_block(const string& stream_name, const string& pos, uint32_t timeout) const = 0;
  };

} // namespace primordia::mud::storage
