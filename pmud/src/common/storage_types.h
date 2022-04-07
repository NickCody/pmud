#pragma once

#include <map>
#include <optional>
#include <vector>
#include <string>
#include <set>

namespace primordia::mud::common {

  using namespace std;

  using StreamRecordFields_t = std::map<std::string, std::string>;

  // base storage types
  //
  using map_t = map<string, string>;
  using list_t = vector<string>;
  using set_t = set<string>;

  // base storage types by name
  //
  using named_map_t = map<string, map_t>;
  using named_list_t = map<string, list_t>;
  using named_set_t = map<string, set_t>;

  // optional base storage types by name
  using opt_string_t = std::optional<std::string>;
  using opt_map_t = std::optional<map_t>;
  using opt_list_t = std::optional<list_t>;
  using opt_set_t = std::optional<set_t>;

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

} // namespace primordia::mud::common