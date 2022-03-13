#pragma once

#include "hiredis/hiredis.h"
#include "storage/redis_types.h"

namespace primordia::mud::storage::redis {

  namespace {
    StreamRecord _stream_record(const string timestamp, redisReply& fields) {

      if (fields.elements % 2 != 0) {
        SPDLOG_ERROR("Field/value array expected to have even dimensions, for timestamp {}", timestamp);
      }

      StreamRecordFields_t stream_fields;

      for (size_t i = 0; i < fields.elements; i += 2) {
        stream_fields[fields.element[i]->str] = fields.element[i + 1]->str;
      }

      return StreamRecord{ timestamp, stream_fields };
    }

    StreamResponse _stream_response(const string stream_name, redisReply* records) {
      StreamRecords_t stream_records;

      for (size_t i = 0; i < records->elements; i++) {
        auto record_elem = records->element[i];

        if (record_elem->elements < 2) {
          SPDLOG_ERROR("Encountered invalid element count while processing stream {}: {}", stream_name, record_elem->elements);
        } else {
          stream_records.push_back(_stream_record(record_elem->element[0]->str, *record_elem->element[1]));
        }
      }

      return StreamResponse{ stream_name, stream_records };
    }
  } // namespace

  vector<StreamResponse> construct_stream_responses(const redisReply& reply) {
    if (reply.type != REDIS_REPLY_ARRAY) {
      SPDLOG_ERROR("Cannot construct stream response from non-array");
      return vector<StreamResponse>();
    }

    vector<StreamResponse> responses;

    for (size_t i = 0; i < reply.elements; i++) {
      auto top_elem = reply.element[i];
      if (top_elem->elements < 2) {
        SPDLOG_ERROR("Encountered invalid element count while constructing stream responses!");
      } else {
        StreamResponse stream = _stream_response(top_elem->element[0]->str, top_elem->element[1]);
        responses.push_back(stream);
      }
    }

    return responses;
  }

} // namespace primordia::mud::storage::redis