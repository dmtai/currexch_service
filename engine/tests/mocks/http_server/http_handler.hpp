#pragma once
#include <gmock/gmock.h>
#include "src/http_server/http_handler.hpp"

namespace engine::http_server {

class MockHttpHandler : public HttpHandlerBase {
 public:
  MOCK_METHOD(void, HandleRequest,
              (const HttpRequestConstPtr& request,
               const HttpResponsePtr& response),
              (const, override));
  MOCK_METHOD(void, Process,
              (const HttpRequestConstPtr& request,
               const HttpResponsePtr& response,
               const ExceptionsHandler& exceptions_handler),
              (const, override));
};

class MockHttpHandlerJson : public HttpHandlerJson {
 public:
  MOCK_METHOD(void, HandleRequest,
              (const HttpRequestConstPtr& request,
               const HttpResponsePtr& response),
              (const, override));
  MOCK_METHOD(void, HandleRequestJson,
              (const HttpRequestConstPtr& request,
               const HttpResponsePtr& response, const Json& body_json),
              (const, override));
};

}  // namespace engine::http_server