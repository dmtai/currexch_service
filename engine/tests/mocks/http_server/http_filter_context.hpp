#pragma once
#include <gmock/gmock.h>
#include "src/http_server/http_filter_context.hpp"

namespace engine::http_server {

class MockHttpFilterContext : public HttpFilterContext {
 public:
  MOCK_METHOD(void, Next,
              (const HttpRequestConstPtr& request,
               const HttpResponsePtr& response),
              (const, override));
  MOCK_METHOD(void, HandleRequest,
              (const HttpRequestConstPtr& request,
               const HttpResponsePtr& response),
              (const, override));
  MOCK_METHOD(void, DoFilter,
              (const HttpRequestConstPtr& request,
               const HttpResponsePtr& response),
              (const, override));
};

}  // namespace engine::http_server