#include "pg_client.hpp"
#include "components/io_context.hpp"

namespace engine::postgres {

namespace {

namespace config {

namespace pg_client_cfg_section {

constexpr std::string_view kName{"pg_client"};
constexpr std::string_view kConnectionInfo{"connection_info"};
constexpr std::string_view kCapacity{"connection_pool_capacity"};
constexpr std::string_view kQueueCapacity{"connection_pool_queue_capacity"};
constexpr std::string_view kIdleTimeout{"connection_pool_idle_timeout"};
constexpr std::string_view kLifespan{"connection_pool_lifespan"};

}  // namespace pg_client_cfg_section

struct PgClientConfig {
  std::string connection_info_;
  size_t capacity_{16};
  size_t queue_capacity_{64};
  std::chrono::seconds idle_timeout_{60};
  std::chrono::hours lifespan_{24};
};

PgClientConfig ParsePgClientConfig(const YamlConfig& yaml_config) {
  PgClientConfig config{};
  if (!yaml_config[pg_client_cfg_section::kConnectionInfo].IsDefined()) {
    throw std::runtime_error{fmt::format(
        "{} setting \"{}\" is required", pg_client_cfg_section::kName,
        pg_client_cfg_section::kConnectionInfo)};
  }
  config.connection_info_ =
      yaml_config[pg_client_cfg_section::kConnectionInfo].as<std::string>();

  if (yaml_config[pg_client_cfg_section::kCapacity].IsDefined()) {
    config.capacity_ =
        yaml_config[pg_client_cfg_section::kCapacity].as<size_t>();
  }
  if (yaml_config[pg_client_cfg_section::kQueueCapacity].IsDefined()) {
    config.queue_capacity_ =
        yaml_config[pg_client_cfg_section::kQueueCapacity].as<size_t>();
  }
  if (yaml_config[pg_client_cfg_section::kIdleTimeout].IsDefined()) {
    config.idle_timeout_ = std::chrono::seconds{
        yaml_config[pg_client_cfg_section::kIdleTimeout].as<size_t>()};
  }
  if (yaml_config[pg_client_cfg_section::kLifespan].IsDefined()) {
    config.lifespan_ = std::chrono::hours{
        yaml_config[pg_client_cfg_section::kLifespan].as<size_t>()};
  }
  return config;
}

}  // namespace config

auto MakeOzoConnetionPool(const config::PgClientConfig& config) {
  const auto connection_info =
      ozo::make_connection_info(config.connection_info_);

  ozo::connection_pool_config connection_pool_config;
  connection_pool_config.capacity = config.capacity_;
  connection_pool_config.queue_capacity = config.queue_capacity_;
  connection_pool_config.idle_timeout = config.idle_timeout_;
  connection_pool_config.lifespan = config.lifespan_;

  return ozo::make_connection_pool(std::move(connection_info),
                                   std::move(connection_pool_config));
}

}  // namespace

PgClient::PgClient(const YamlConfig& config,
                   engine::components::Components& components)
    : io_context_{components.Get<engine::components::ServerIOContext>()},
      connection_pool_{
          MakeOzoConnetionPool(config::ParsePgClientConfig(config))} {}

}  // namespace engine::postgres