#pragma once

#include <map>
#include <vector>
#include <string>

#include "common/global_type_id.h"

namespace primordia::mud::storage {
  using namespace std;

  struct StreamRecord {
    string timestamp;
    StreamRecordFields_t fields;
  };

  using StreamRecords_t = vector<StreamRecord>;

  struct StreamResponse {
    string stream_name;
    StreamRecords_t records;
  };

  using StreamResponses_t = vector<StreamResponse>;

} // namespace primordia::mud::storage