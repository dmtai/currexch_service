#pragma once
#include <gmock/gmock.h>
#include "src/http_server/http_response.hpp"

namespace engine::http_server {

class MockHttpResponse : public HttpResponse {
 public:
  MOCK_METHOD(void, SetContentType, (ContentType content_type), (override));
  MOCK_METHOD(void, SetKeepAlive, (bool keep_alive), (override));
  MOCK_METHOD(void, SetStatusCode, (HttpStatus http_status), (override));
  MOCK_METHOD(void, SetHttpVersion, (unsigned http_version), (override));
  MOCK_METHOD(void, Send, (const std::string& data), (override));
  MOCK_METHOD(void, SendFile, (const std::filesystem::path& path), (override));
  MOCK_METHOD(void, Close, (), (override));
};

}  // namespace engine::http_server