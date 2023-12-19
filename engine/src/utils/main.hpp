#pragma once

#include "http_server/server_builder.hpp"

namespace engine::utils {

class Main {
 public:
  Main(const int argc, const char* const argv[]);
  int Run() noexcept;

  template <typename T>
  Main& AddHandler() {
    server_builder_.AddHandler<T>();
    return *this;
  }

  template <typename T>
  Main& AddFilter() {
    server_builder_.AddFilter<T>();
    return *this;
  }

  template <typename T>
  Main& AddComponent() {
    server_builder_.AddComponent<T>();
    return *this;
  }

 private:
  const int argc_;
  const char* const* argv_;
  engine::http_server::ServerBuilder server_builder_;
};

}  // namespace engine::utils