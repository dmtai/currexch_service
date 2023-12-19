#include "components.hpp"

namespace engine::components {

Components::Components(YamlConfig config, ComponentsFactoryPtr factory)
    : config_{std::move(config)}, components_factory_{std::move(factory)} {}

void Components::Add(std::string_view key, ComponentBasePtr value) {
  if (!value) {
    throw std::runtime_error(fmt::format(
        "Trying to add an invalid value with key \"{}\" to the Components",
        key));
  }
  components_.insert({ComponentName{key}, std::move(value)});
}

bool Components::Contains(const ComponentName& name) const {
  return components_.contains(name);
}

void Components::ForEach(const ForEachCallback& cb) {
  for (auto& [name, component] : components_) {
    cb(name, component);
  }
}

}  // namespace engine::components