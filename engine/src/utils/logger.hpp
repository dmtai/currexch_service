#pragma once

#include <iostream>
#include <optional>
#include <string>
#include "fmt/core.h"
#include "spdlog/spdlog.h"

namespace engine::logger {

using SpdlogPtr = std::shared_ptr<spdlog::logger>;
using LoggerBasePtr = std::shared_ptr<class LoggerBase>;

class LoggerBase {
 public:
  static const std::string kDefaultLogFormat;

  using Level = enum : int {
    trace = spdlog::level::level_enum::trace,
    debug = spdlog::level::level_enum::debug,
    info = spdlog::level::level_enum::info,
    warn = spdlog::level::level_enum::warn,
    error = spdlog::level::level_enum::err,
    critical = spdlog::level::level_enum::critical,
    off = spdlog::level::level_enum::off
  };

  explicit LoggerBase(SpdlogPtr logger);

  Level GetLevel() const;
  void SetLevel(Level lvl);
  void SetLogFormat(const std::string& fmt);
  void Flush();

  template <typename... Args>
  constexpr void Log(spdlog::source_loc loc, Level lvl,
                     const fmt::format_string<const Args&...>& fmt,
                     const Args&... args) {
    logger_->log(loc, static_cast<spdlog::level::level_enum>(lvl), fmt,
                 args...);
  }

 private:
  const SpdlogPtr logger_;
};

using LogLevelOpt = std::optional<LoggerBase::Level>;

LogLevelOpt ToLogLevelEnum(std::string_view lvl);
LoggerBase& GetDefaultLogger();
void SetDefaultLogger(LoggerBasePtr l);

class Logger : public LoggerBase {
 public:
  static const std::string kName;
  Logger(const std::string& log_path, Level lvl);
};

LoggerBasePtr MakeLogger(const std::string& log_path, Logger::Level lvl);

class StdoutLogger : public LoggerBase {
 public:
  static const std::string kName;
  explicit StdoutLogger(const std::string& logger_name = kName);
};

#define LOG_LEVEL(LEVEL) engine::logger::Logger::LEVEL

#define CHECK_LOG_LEVEL(LOGGER, LEVEL) (LOG_LEVEL(LEVEL) >= (LOGGER).GetLevel())

#define LOG_TO_LOGGER(LOGGER, LEVEL, ...)                            \
  do {                                                               \
    if (CHECK_LOG_LEVEL(LOGGER, LEVEL)) {                            \
      LOGGER.Log(::spdlog::source_loc{__FILE__, __LINE__, __func__}, \
                 LOG_LEVEL(LEVEL), __VA_ARGS__);                     \
    }                                                                \
  } while (0)

#define GET_LOGGER() engine::logger::GetDefaultLogger()
#define LOG(LEVEL, ...) LOG_TO_LOGGER(GET_LOGGER(), LEVEL, ##__VA_ARGS__)

}  // namespace engine::logger