#pragma once
#include <gmock/gmock.h>
#include "src/http_server/http_filter.hpp"

namespace engine::http_server {

class MockHttpFilter : public HttpFilterBase {
 public:
  MOCK_METHOD(void, DoFilter,
              (const HttpRequestConstPtr& request,
               const HttpResponsePtr& response,
               const HttpFilterContext& filter_ctx),
              (const, override));
  MOCK_METHOD(void, Process,
              (const HttpRequestConstPtr& request,
               const HttpResponsePtr& response,
               const HttpFilterContext& filter_ctx,
               const ExceptionsHandler& exceptions_handler),
              (const, override));
};

}  // namespace engine::http_server