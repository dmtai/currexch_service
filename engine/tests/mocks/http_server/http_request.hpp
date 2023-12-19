#pragma once
#include <gmock/gmock.h>
#include "src/http_server/http_request.hpp"

namespace engine::http_server {

class MockHttpRequest : public HttpRequest {
 public:
  MOCK_METHOD(std::string_view, GetUri, (), (const, override));
  MOCK_METHOD(HttpMethod, GetMethod, (), (const, override));
  MOCK_METHOD(const std::string&, GetPath, (), (const, override));
  MOCK_METHOD(HttpRequest::Query, GetQuery, (), (const, override));
  MOCK_METHOD(const router::MatchResult&, GetMatchResult, (),
              (const, override));
  MOCK_METHOD(const std::string&, GetBody, (), (const, noexcept, override));
  MOCK_METHOD(bool, GetKeepAlive, (), (const, override));
  MOCK_METHOD(unsigned, GetHttpVersion, (), (const, noexcept, override));
  MOCK_METHOD(void, SetMatchResult, (router::MatchResult match_result),
              (override));
};

}  // namespace engine::http_server