#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>
#include "utils/config.hpp"
#include "utils/logger.hpp"

namespace engine::http_server::config {

struct General {
  std::string address_{"0.0.0.0"};
  size_t port_{8080};
  size_t session_timeout_{10000};
  std::string log_path_{"./"};
  logger::LoggerBase::Level log_level_{logger::LoggerBase::trace};
};

struct HttpHandler {
  using HttpFilterName = std::string;
  using HttpFilterPair = std::pair<HttpFilterName, YamlConfig>;
  using HttpFiltersVec = std::vector<HttpFilterPair>;

  std::string path_;
  std::string method_;
  YamlConfig yaml_config_;
  HttpFiltersVec filters_;
};

struct Config {
  using HttpHandlerName = std::string;
  using HttpHandlersMap = std::unordered_map<HttpHandlerName, HttpHandler>;
  using HttpFilterNames = std::unordered_set<std::string>;

  Config() = default;
  Config(General general, HttpHandlersMap http_hanlders_map,
         HttpFilterNames filter_names, YamlConfig components);

  General general_;
  HttpHandlersMap http_handlers_;
  HttpFilterNames filter_names_;
  YamlConfig components_;
};

using ConfigConstPtr = std::shared_ptr<const Config>;

Config ParseConfig(const YamlConfig& path);

}  // namespace engine::http_server::config

namespace YAML {

template <>
struct convert<engine::http_server::config::General> {
  static bool decode(const Node& node,
                     engine::http_server::config::General& rhs);
};

}  // namespace YAML