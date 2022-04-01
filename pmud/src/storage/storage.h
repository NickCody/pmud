#pragma once

#include <arpa/inet.h>
#include <string>
#include <ctime>
#include <limits.h>
#include <map>
#include <memory>
#include <netdb.h>
#include <sstream>
#include <string>
#include <hiredis/hiredis.h>
#include <spdlog/spdlog.h>

#include "common/util.h"
#include "storage/storage_types.h"

namespace primordia::mud::storage {

  using namespace std;

  class Storage {
  public:
    virtual ~Storage(){};
    virtual bool init() = 0;
    virtual bool value_store(const string& key, const string& value) = 0;
    virtual bool map_store(const string& map_name, const string& key, const string& value) = 0;
    virtual bool list_store(const string& list_name, const string& value) = 0;
    virtual bool set_store(const string& set_name, const string& value) = 0;
    virtual bool del_key(const string& key) = 0;
    virtual bool event_store(const string& event_name, const StreamRecordFields_t& fields) = 0;
    virtual bool stream_store(const string& map_name, const StreamRecordFields_t& fields) = 0;
    virtual vector<StreamResponse> read_stream_raw(const string& command) = 0;
    virtual vector<StreamResponse> read_stream_block(const string& stream_name, const string& pos, uint32_t timeout) = 0;
  };

} // namespace primordia::mud::storage