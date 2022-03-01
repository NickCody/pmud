#pragma once

#include <caf/all.hpp>
#include <fmt/format.h>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <memory>

#define ALOG_INFO(f, ...) primordia::mud::logger::Logger::SPDLOG_INFO(fmt::format(f, __VA_ARGS__))
#define ALOG_DEBUG(f, ...) primordia::mud::logger::Logger::SPDLOG_DEBUG(fmt::format(f, __VA_ARGS__))
#define ALOG_ERROR(f, ...) primordia::mud::logger::Logger::SPDLOG_ERROR(fmt::format(f, __VA_ARGS__))
#define ALOG_WARN(f, ...) primordia::mud::logger::Logger::SPDLOG_WARN(fmt::format(f, __VA_ARGS__))

#define ALOG_INFO_1(f) primordia::mud::logger::Logger::SPDLOG_INFO(f)
#define ALOG_DEBUG_1(f) primordia::mud::logger::Logger::SPDLOG_DEBUG(f)
#define ALOG_ERROR_1(f) primordia::mud::logger::Logger::SPDLOG_ERROR(f)
#define ALOG_WARN_1(f) primordia::mud::logger::Logger::SPDLOG_WARN(f)

namespace primordia::mud::logger {

  using namespace caf;
  using namespace std;
  using namespace std::chrono;

  class Logger {
    inline static strong_actor_ptr _actor;

    static behavior logging_actor(event_based_actor* /*self*/) {
      return {};
    }

  public:
    ~Logger() {}

    static void init(actor_system& sys) {
      _actor = actor_cast<strong_actor_ptr>(sys.spawn(logging_actor));
    }

    static void SPDLOG_INFO(const string& message) {
      aout(actor_cast<event_based_actor*>(_actor)) << utc_time() << " INFO " << message << endl;
    }

    static void SPDLOG_ERROR(const string& message) {
      aout(actor_cast<event_based_actor*>(_actor)) << utc_time() << " ERROR " << message << endl;
    }

    static void SPDLOG_WARN(const string& message) {
      aout(actor_cast<event_based_actor*>(_actor)) << utc_time() << " WARN " << message << endl;
    }

    static void SPDLOG_DEBUG(const string& message) {
      aout(actor_cast<event_based_actor*>(_actor)) << utc_time() << " DEBUG " << message << endl;
    }

    static string utc_time() {
      auto now = system_clock::now();
      auto millis = duration_cast<milliseconds>(now.time_since_epoch()).count() % 1000;

      time_t tt = chrono::system_clock::to_time_t(now);
      tm tm = *gmtime(&tt);
      stringstream ss;
      ss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
      ss << "." << millis;
      return ss.str();
    }
  };

} // namespace primordia::mud::logger