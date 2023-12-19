#pragma once
#include <functional>
#include <optional>

namespace engine::utils {

template <class T>
struct OptRef : public std::optional<std::reference_wrapper<T>> {
  OptRef(T& t) : std::optional<std::reference_wrapper<T>>(t) {}
  OptRef() = default;

  template <class From>
  explicit OptRef(OptRef<From> rhs) {
    if (rhs.has_value()) {
      *this = rhs.ref();
    }
  }

  template <class From>
  OptRef<T>& operator=(OptRef<From> rhs) {
    this->reset();
    if (rhs.has_value()) {
      *this = rhs.ref();
    }
    return *this;
  }

  T* operator->() {
    T& ref = **this;
    return &ref;
  }

  const T* operator->() const {
    const T& ref = **this;
    return &ref;
  }

  T* Ptr() {
    if (this->has_value()) {
      T& ref = **this;
      return &ref;
    }

    return nullptr;
  }

  const T* Ptr() const {
    if (this->has_value()) {
      const T& ref = **this;
      return &ref;
    }

    return nullptr;
  }

  T& Ref() { return **this; }

  const T& Ref() const { return **this; }
};

}  // namespace engine::utils