#include "config.hpp"
#include <fmt/core.h>
#include <filesystem>

namespace engine::http_server::config {

namespace {

constexpr std::string_view kHttpFiltersSection{"filters"};
constexpr std::string_view kComponentsSections{"components"};

namespace ganeral_section {

constexpr std::string_view kName{"general"};
constexpr std::string_view kAddress{"address"};
constexpr std::string_view kPort{"port"};
constexpr std::string_view kSessionTimeout{"session_timeout"};
constexpr std::string_view kLogPath{"log_path"};
constexpr std::string_view kLogLevel{"log_level"};

};  // namespace ganeral_section

namespace http_handlers_section {

constexpr std::string_view kName{"http-handlers"};
constexpr std::string_view kPath{"path"};
constexpr std::string_view kMethod{"method"};

};  // namespace http_handlers_section

using HandlersAndFilterNames =
    std::pair<Config::HttpHandlersMap, Config::HttpFilterNames>;

void ParseHttpFiltersConfig(const YamlConfig& config, HttpHandler& http_handler,
                            Config::HttpFilterNames& filter_names) {
  for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
    const auto filter_name = it->first.as<std::string>();
    filter_names.insert(filter_name);
    http_handler.filters_.push_back({std::move(filter_name), it->second});
  }
}

HandlersAndFilterNames ParseHttpHandlersConfig(const YamlConfig& config) {
  Config::HttpHandlersMap http_handlers_map;
  Config::HttpFilterNames filter_names;

  for (YAML::const_iterator it = config.begin(); it != config.end(); ++it) {
    const auto handler_name = it->first.as<std::string>();
    if (!it->second[http_handlers_section::kPath].IsDefined()) {
      throw std::runtime_error{
          fmt::format("Error reading \"{}\" configuration. "
                      "\"{}\" setting is required",
                      handler_name, http_handlers_section::kPath)};
    }
    if (!it->second[http_handlers_section::kMethod].IsDefined()) {
      throw std::runtime_error{
          fmt::format("Error reading \"{}\" configuration. "
                      "\"{}\" setting is required",
                      handler_name, http_handlers_section::kMethod)};
    }

    HttpHandler handler{};
    handler.path_ = it->second[http_handlers_section::kPath].as<std::string>();
    handler.method_ =
        it->second[http_handlers_section::kMethod].as<std::string>();
    handler.yaml_config_ = it->second;

    if (it->second[kHttpFiltersSection].IsDefined()) {
      ParseHttpFiltersConfig(it->second[kHttpFiltersSection], handler,
                             filter_names);
    }

    http_handlers_map.insert({std::move(handler_name), std::move(handler)});
  }

  return {http_handlers_map, filter_names};
}

}  // namespace

Config::Config(General general, HttpHandlersMap http_hanlders_map,
               HttpFilterNames filter_names, YamlConfig components)
    : general_{std::move(general)},
      http_handlers_{std::move(http_hanlders_map)},
      filter_names_{std::move(filter_names)},
      components_{std::move(components)} {}

Config ParseConfig(const YamlConfig& config) {
  General general{};
  if (config[ganeral_section::kName].IsDefined()) {
    general = config[ganeral_section::kName].as<General>();
  }

  if (!config[http_handlers_section::kName].IsDefined()) {
    return {std::move(general),
            Config::HttpHandlersMap{},
            Config::HttpFilterNames{},
            {}};
  }

  const auto components = config[kComponentsSections];
  const auto [handlers, filter_names] =
      ParseHttpHandlersConfig(config[http_handlers_section::kName]);

  return {std::move(general), std::move(handlers), std::move(filter_names),
          std::move(components)};
}

}  // namespace engine::http_server::config

namespace YAML {

bool convert<engine::http_server::config::General>::decode(
    const Node& node, engine::http_server::config::General& rhs) {
  using namespace engine::http_server::config;
  if (!node.IsMap()) {
    return false;
  }
  if (node[ganeral_section::kAddress].IsDefined()) {
    rhs.address_ = node[ganeral_section::kAddress].as<std::string>();
  }
  if (node[ganeral_section::kPort].IsDefined()) {
    rhs.port_ = node[ganeral_section::kPort].as<size_t>();
  }
  if (node[ganeral_section::kSessionTimeout].IsDefined()) {
    rhs.session_timeout_ = node[ganeral_section::kSessionTimeout].as<size_t>();
  }
  if (node[ganeral_section::kLogPath].IsDefined()) {
    rhs.log_path_ = node[ganeral_section::kLogPath].as<std::string>();
  }
  if (node[ganeral_section::kLogLevel].IsDefined()) {
    const auto log_level_string =
        node[ganeral_section::kLogLevel].as<std::string>();
    const auto log_level = engine::logger::ToLogLevelEnum(log_level_string);
    if (!log_level) {
      throw std::runtime_error{
          fmt::format("Error reading \"general.{}\" configuration setting. "
                      "Log-level \"{}\" is undefined",
                      ganeral_section::kLogLevel, log_level_string)};
    }
    rhs.log_level_ = *log_level;
  }
  return true;
}

}  // namespace YAML