#include "pg_cache.hpp"
#include <memory>

namespace engine::postgres {

namespace config {

PgCacheConfig ParsePgCacheConfig(const YamlConfig& yaml_config) {
  PgCacheConfig config{};
  if (yaml_config["update_timeout"].IsDefined()) {
    config.update_timeout_ =
        std::chrono::milliseconds{yaml_config["update_timeout"].as<size_t>()};
  }
  if (yaml_config["full_update_timeout"].IsDefined()) {
    config.update_timeout_ = std::chrono::milliseconds{
        yaml_config["full_update_timeout"].as<size_t>()};
  }
  return config;
}

}  // namespace config

}  // namespace engine::postgres