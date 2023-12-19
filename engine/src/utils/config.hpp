#pragma once

#include <optional>
#include <string>
#include "yaml-cpp/yaml.h"

namespace engine {

using YamlConfig = YAML::Node;
using YamlConfigOpt = std::optional<YamlConfig>;

namespace config {

YamlConfig RetrieveValidOrEmptyConfig(std::string_view name,
                                      const YamlConfig& config);

}  // namespace config

}  // namespace engine