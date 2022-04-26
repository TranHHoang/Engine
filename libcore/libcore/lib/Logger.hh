#pragma once
#include <chrono>
#include <string_view>

#include <fmt/chrono.h>
#include <fmt/core.h>

#include <libcore/lib/Memory.hh>

namespace Engine {
class Logger {
public:
  enum class Level { Debug, Info, Warn, Error };
  static std::string_view toString(Level level) {
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
    auto now = std::chrono::system_clock::now();
    fmt::print("{}[{:%H:%M:%S}] INFO: ", logColor(Level::Info), now);
    fmt::print(format, std::forward<Args>(args)...);
    fmt::print("\033[0m\n");
  }

  template <typename... Args>
  static void error(fmt::format_string<Args...> format, Args&&... args) {
    auto now = std::chrono::system_clock::now();
    fmt::print("{}[{:%H:%M:%S}] ERROR: ", logColor(Level::Error), now);
    fmt::print(format, std::forward<Args>(args)...);
    fmt::print("\033[0m\n");
  }

private:
  static std::string_view logColor(Level level) {
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
};
} // namespace Engine