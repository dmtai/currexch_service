#pragma once

#include <memory>

namespace engine::components {

class ComponentBase {
 public:
  ComponentBase() = default;
  ComponentBase(const ComponentBase&) = delete;
  ComponentBase& operator=(const ComponentBase&) = delete;
  virtual ~ComponentBase() = default;
};

using ComponentBasePtr = std::shared_ptr<ComponentBase>;

}  // namespace engine::components