#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include "utils/config.hpp"
#include "utils/factory.hpp"

namespace engine::components {

using ComponentsPtr = std::shared_ptr<class Components>;
using ComponentsConstPtr = std::shared_ptr<const class Components>;

class ComponentsFactory
    : public utils::FactoryBase<class ComponentBase, const YamlConfig&,
                                Components&> {};

using ComponentsFactoryPtr = std::shared_ptr<ComponentsFactory>;

}  // namespace engine::components