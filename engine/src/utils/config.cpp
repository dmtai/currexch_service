#include "config.hpp"

namespace engine {

namespace config {

YamlConfig RetrieveValidOrEmptyConfig(std::string_view name,
                                      const YamlConfig& config) {
  if (!config.IsDefined()) {
    return {};
  }
  const auto res = config[name];
  if (!res.IsDefined()) {
    return {};
  }
  return res;
}

}  // namespace config

}  // namespace engine