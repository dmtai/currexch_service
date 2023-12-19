#include "logger.hpp"
#include "bimap.hpp"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace engine::logger {

namespace {

LoggerBasePtr& StaticDefaultLogger() {
  static LoggerBasePtr logger{std::make_shared<StdoutLogger>()};
  return logger;
}

namespace log_levels {

constexpr std::string_view kTrace{"trace"};
constexpr std::string_view kDebug{"debug"};
constexpr std::string_view kInfo{"info"};
constexpr std::string_view kWarn{"warn"};
constexpr std::string_view kError{"error"};
constexpr std::string_view kCritical{"critical"};
constexpr std::string_view kOff{"off"};

}  // namespace log_levels

using LogLevelEnumByString =
    utils::CaseInsensitiveUnorderedBiMap<std::string_view, LoggerBase::Level>;

const auto log_level_enum_by_string =
    engine::utils::MakeBimap<LogLevelEnumByString>(
        {{log_levels::kTrace, LoggerBase::Level::trace},
         {log_levels::kDebug, LoggerBase::Level::debug},
         {log_levels::kInfo, LoggerBase::Level::info},
         {log_levels::kWarn, LoggerBase::Level::warn},
         {log_levels::kError, LoggerBase::Level::error},
         {log_levels::kCritical, LoggerBase::Level::critical},
         {log_levels::kOff, LoggerBase::Level::off}});

}  // namespace

const std::string LoggerBase::kDefaultLogFormat =
    "[%Y-%m-%d %T.%e][%t][%l] [%g:%#] %v";

LoggerBase::LoggerBase(SpdlogPtr logger) : logger_(logger) {
  SetLogFormat(kDefaultLogFormat);
}

void LoggerBase::SetLogFormat(const std::string& fmt) {
  logger_->set_pattern(fmt);
}

void LoggerBase::Flush() { logger_->flush(); }

LoggerBase::Level LoggerBase::GetLevel() const {
  return static_cast<Level>(logger_->level());
}

void LoggerBase::SetLevel(Level lvl) {
  logger_->set_level(static_cast<spdlog::level::level_enum>(lvl));
}

const std::string Logger::kName = "main";

Logger::Logger(const std::string& log_path, Level lvl)
    : LoggerBase{
          spdlog::basic_logger_mt<spdlog::async_factory>(kName, log_path)} {
  SetLevel(lvl);
}

const std::string StdoutLogger::kName = "stdout";

StdoutLogger::StdoutLogger(const std::string& logger_name)
    : LoggerBase{spdlog::stdout_color_mt(logger_name)} {
  SetLevel(trace);
}

LoggerBase& GetDefaultLogger() { return *StaticDefaultLogger(); }

void SetDefaultLogger(LoggerBasePtr logger) {
  StaticDefaultLogger() = std::move(logger);
}

LoggerBasePtr MakeLogger(const std::string& log_path, Logger::Level lvl) {
  return std::make_shared<Logger>(log_path, lvl);
}

LogLevelOpt ToLogLevelEnum(std::string_view lvl) {
  const auto it = log_level_enum_by_string.left.find(lvl);
  if (it != log_level_enum_by_string.left.end()) {
    return {it->second};
  }
  return {};
}

}  // namespace engine::logger