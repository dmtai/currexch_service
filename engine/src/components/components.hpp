#pragma once

#include <fmt/core.h>
#include <memory>
#include "component_base.hpp"
#include "factory.hpp"
#include "utils/config.hpp"
#include "utils/net.hpp"

namespace engine::components {

// circular component dependencies are not supported
class Components {
 public:
  using ComponentName = std::string;
  using ComponentsMap = std::unordered_map<ComponentName, ComponentBasePtr>;
  using ForEachCallback =
      std::function<void(const ComponentName&, const ComponentBasePtr&)>;

  Components(const Components&) = delete;
  Components& operator=(const Components&) = delete;
  Components(YamlConfig config, ComponentsFactoryPtr factory);

  void Add(std::string_view key, ComponentBasePtr value);
  bool Contains(const ComponentName& name) const;
  void ForEach(const ForEachCallback& cb);

  template <typename T>
  std::shared_ptr<T> Get(const std::string& component_name = std::string{
                             T::kName}) {
    const auto it = components_.find(component_name);
    if (it != components_.end()) {
      const auto res = std::dynamic_pointer_cast<T>(it->second);
      if (!res) {
        throw std::runtime_error{
            fmt::format("Component \"{}\" isn't type \"{}\"", component_name,
                        typeid(T).name())};
      }
      return res;
    }
    const auto new_component = components_factory_->Create(
        component_name,
        engine::config::RetrieveValidOrEmptyConfig(component_name, config_),
        *this);
    if (!new_component) {
      throw std::runtime_error{fmt::format(
          "Component \"{}\" isn't registered in the factory", component_name)};
    }
    const auto res = std::dynamic_pointer_cast<T>(new_component);
    if (!res) {
      throw std::runtime_error{fmt::format("Component \"{}\" isn't type \"{}\"",
                                           component_name, typeid(T).name())};
    }
    Add(component_name, new_component);
    return res;
  }

  template <typename T>
  void ForEachComponentOfType(const std::function<void(T&)>& cb) {
    const auto for_each_component = [&](const auto& name,
                                        const auto& component) {
      const auto res = std::dynamic_pointer_cast<T>(component);
      if (!res) {
        return;
      }
      cb(*res);
    };
    ForEach(for_each_component);
  }

 private:
  const YamlConfig config_;
  const ComponentsFactoryPtr components_factory_;
  ComponentsMap components_;
};

using ComponentsPtr = std::shared_ptr<Components>;
using ComponentsConstPtr = std::shared_ptr<const Components>;

}  // namespace engine::components