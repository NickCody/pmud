#pragma once

#include "hiredis/hiredis.h"
#include "storage/redis_types.h"

namespace primordia::mud::storage {

  StreamRecord construct_stream_record(const string timestamp, redisReply* fields) {
    StreamRecord record;
    record.timestamp = timestamp;
    size_t i = 0;
    while (i < fields->elements) {
      record.fields[fields->element[i]->str] = fields->element[i + 1]->str;
      i += 2;
    }

    return record;
  }

  StreamResponse construct_stream_response(const string stream_name, redisReply* records) {
    StreamResponse stream_response;
    stream_response.stream_name = stream_name;

    for (size_t i = 0; i < records->elements; i++) {
      auto record_elem = records->element[i];
      StreamRecord record = construct_stream_record(record_elem->element[0]->str, record_elem->element[1]);
      stream_response.records.push_back(record);
    }

    return stream_response;
  }

  vector<StreamResponse> construct_streams(redisReply* reply) {
    vector<StreamResponse> responses;

    for (size_t i = 0; i < reply->elements; i++) {
      auto top_elem = reply->element[i];
      StreamResponse stream = construct_stream_response(top_elem->element[0]->str, top_elem->element[1]);
      responses.push_back(stream);
    }
    return responses;
  }

} // namespace primordia::mud::storage