#pragma once
#include <gmock/gmock.h>
#include "src/http_server/server_context.hpp"

namespace engine::http_server {

class MockServerContext : public ServerContext {
 public:
  MOCK_METHOD(const router::Router&, GetRouter, (), (const, noexcept));
  MOCK_METHOD(const config::Config&, GetConfig, (), (const, noexcept));
  MOCK_METHOD(const HttpFilterContextsMap&, GetHttpFilterContextsMap, (),
              (const, noexcept));
  MOCK_METHOD(const ExceptionsHandler&, GetExceptionsHandler, (),
              (const, noexcept));
};

}  // namespace engine::http_server