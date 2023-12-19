#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "http_request.hpp"
#include "http_response.hpp"

namespace engine::http_server {

using HttpFilterContextConstPtr =
    std::shared_ptr<const class HttpFilterContext>;

class HttpFilterBase {
 public:
  virtual ~HttpFilterBase() = default;
  virtual void DoFilter(const HttpRequestConstPtr& request,
                        const HttpResponsePtr& response,
                        const HttpFilterContext& filter_ctx) const = 0;
  virtual void Process(const HttpRequestConstPtr& request,
                       const HttpResponsePtr& response,
                       const HttpFilterContext& filter_ctx,
                       const ExceptionsHandler& exceptions_handler) const;
};

using HttpFilterBasePtr = std::shared_ptr<HttpFilterBase>;
using HttpFilterBaseConstPtr = std::shared_ptr<const HttpFilterBase>;

}  // namespace engine::http_server