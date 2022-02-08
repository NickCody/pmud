#pragma once

#include <caf/all.hpp>
#include <fmt/format.h>
#include <sstream>
#include <chrono>
#include <iomanip>

#define LOG_INFO(f, ...) primordia::mud::logger::Logger::log_info(fmt::format(f, __VA_ARGS__))
#define LOG_DEBUG(f, ...) primordia::mud::logger::Logger::log_debug(fmt::format(f, __VA_ARGS__))
#define LOG_ERROR(f, ...) primordia::mud::logger::Logger::log_error(fmt::format(f, __VA_ARGS__))
#define LOG_WARN(f, ...) primordia::mud::logger::Logger::log_warn(fmt::format(f, __VA_ARGS__))

#define LOG_INFO_1(f) primordia::mud::logger::Logger::log_info(f)
#define LOG_DEBUG_1(f) primordia::mud::logger::Logger::log_debug(f)
#define LOG_ERROR_1(f) primordia::mud::logger::Logger::log_error(f)
#define LOG_WARN_1(f) primordia::mud::logger::Logger::log_warn(f)

namespace primordia::mud::logger {

  using namespace caf;
  using namespace std;
  using namespace std::chrono;

  class Logger {
    inline static scoped_actor* _actor;

  public:
    static void init(scoped_actor* actor) {
      _actor = actor;
    }

    static void log_info(const string& message) {
      aout(*_actor) << utc_time() << " INFO " << message << endl;
    }

    static void log_error(const string& message) {
      aout(*_actor) << utc_time() << " ERROR " << message << endl;
    }

    static void log_warn(const string& message) {
      aout(*_actor) << utc_time() << " WARN " << message << endl;
    }

    static void log_debug(const string& message) {
      aout(*_actor) << utc_time() << " DEBUG " << message << endl;
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