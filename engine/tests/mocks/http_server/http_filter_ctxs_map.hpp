#pragma once
#include <gmock/gmock.h>
#include "src/http_server/http_filter_ctxs_map.hpp"

namespace engine::http_server {

class MockHttpFilterContextsMap : public HttpFilterContextsMap {
 public:
  MOCK_METHOD(void, AddFront,
              (const HandlerName& name, HttpFilterContextConstPtr http_filter),
              (override));
  MOCK_METHOD(bool, HasFilters, (const HandlerName& handler_name),
              (const, override));
  MOCK_METHOD(void, DoFiltersAndHandler,
              (const HandlerName& name, const HttpRequestConstPtr& request,
               const HttpResponsePtr& response),
              (const, override));
};

}  // namespace engine::http_server