#pragma once
#include <gmock/gmock.h>
#include "src/http_server/http_session.hpp"

namespace engine::http_server {

class MockHttpSession {
 public:
  MOCK_METHOD(void, Run, ());
  MOCK_METHOD(void, Close, ());
  MOCK_METHOD(void, Write, (http::response<http::string_body> http_response));
  MOCK_METHOD(void, Write, (http::response<http::file_body> response));
};

}  // namespace engine::http_server