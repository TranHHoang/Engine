#pragma once
#include <chrono>
#include <string_view>

#include <fmt/chrono.h>
#include <fmt/core.h>
#ifdef ANDROID
#include <android/log.h>
#endif
#include <libcore/lib/Memory.hh>

namespace Engine {
class Logger {
public:
  enum class Level { Debug, Info, Warn, Error };
  static constexpr std::string_view toString(Level level) {
    switch (level) {
    case Level::Debug:
      return "Debug";
    case Level::Error:
      return "Error";
    case Level::Info:
      return "Info";
    case Level::Warn:
      return "Warn";
    }
    return "";
  }

public:
  template <typename... Args>
  static void info(fmt::format_string<Args...> format, Args&&... args) {
    print(Level::Info, format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static void error(fmt::format_string<Args...> format, Args&&... args) {
    print(Level::Error, format, std::forward<Args>(args)...);
  }

private:
  static constexpr std::string_view logColor(Level level) {
    switch (level) {
    case Level::Debug:
      return "\033[30m";
    case Level::Error:
      return "\033[31m";
    case Level::Info:
      return "\033[32m";
    case Level::Warn:
      return "\033[33m";
    }
    return "";
  }

#ifdef ANDROID
  static constexpr android_LogPriority androidLogLevel(Level level) {
    switch (level) {
    case Level::Info:
      return ANDROID_LOG_INFO;
    case Level::Error:
      return ANDROID_LOG_ERROR;
    case Level::Debug:
      return ANDROID_LOG_DEBUG;
    default:
      return ANDROID_LOG_WARN;
    }
  }
#endif

  template <typename... Args>
  static inline void
  print(Level level, fmt::format_string<Args...> format, Args&&... args) {
    std::string_view logLevel =
#ifdef ANDROID
        "";
#else
        logColor(level);
#endif
    std::string log = fmt::format("{}[{:%H:%M:%S}] {}: ",
                                  logLevel,
                                  std::chrono::system_clock::now(),
                                  toString(level));
    log += fmt::format(format, std::forward<Args>(args)...);

#ifdef ANDROID
    __android_log_print(androidLogLevel(level), "Engine", "%s", log.c_str());
#else
    fmt::vprint(log, {});
    fmt::print("\033[0m\n");
#endif
  }
};
} // namespace Engine