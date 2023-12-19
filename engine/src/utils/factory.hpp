#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>

namespace engine::utils {

template <typename BaseObjectType, typename... ConstructorArgs>
class FactoryBase {
 public:
  using BaseObjectTypePtr = std::shared_ptr<BaseObjectType>;
  using ObjectCreationCb =
      std::function<BaseObjectTypePtr(ConstructorArgs&&...)>;
  using ObjectName = std::string;
  using Storage = std::unordered_map<ObjectName, ObjectCreationCb>;

  template <typename T>
  void Add(std::string_view name = T::kName) {
    static_assert(std::is_base_of_v<BaseObjectType, T>,
                  "T must be a subclass of BaseObjectType");
    storage_.insert({std::string{name}, [](ConstructorArgs&&... args) {
                       return std::make_shared<T>(
                           std::forward<ConstructorArgs>(args)...);
                     }});
  }

  template <typename... Args>
  [[nodiscard]] BaseObjectTypePtr Create(const typename Storage::key_type& name,
                                         Args&&... args) const {
    const auto it = storage_.find(name);
    if (it != storage_.end()) {
      return it->second(std::forward<Args>(args)...);
    }
    return nullptr;
  }

  template <typename T>
  void ForEach(T&& cb) const {
    for (const auto& [key, value] : storage_) {
      cb(key, value);
    }
  }

 private:
  Storage storage_;
};

}  // namespace engine::utils