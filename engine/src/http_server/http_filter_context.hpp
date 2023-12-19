#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "http_filter.hpp"
#include "http_handler.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "utils/opt_ref.hpp"

namespace engine::http_server {

using HttpFilterContextPtr = std::shared_ptr<class HttpFilterContext>;
using HttpFilterContextConstPtr =
    std::shared_ptr<const class HttpFilterContext>;
using ExceptionsHandlerConstPtr =
    std::shared_ptr<const class ExceptionsHandler>;

class HttpFilterContext {
 public:
  virtual ~HttpFilterContext() = default;
  virtual void Next(const HttpRequestConstPtr& request,
                    const HttpResponsePtr& response) const = 0;
  virtual void HandleRequest(const HttpRequestConstPtr& request,
                             const HttpResponsePtr& response) const = 0;
  virtual void DoFilter(const HttpRequestConstPtr& request,
                        const HttpResponsePtr& response) const = 0;
};

class HttpFilterContextImpl
    : public HttpFilterContext,
      public std::enable_shared_from_this<HttpFilterContextImpl> {
 public:
  HttpFilterContextImpl(HttpFilterBaseConstPtr http_filter,
                        HttpFilterContextConstPtr next_http_filter_ctx,
                        HttpHandlerBaseConstPtr http_handler_base,
                        ExceptionsHandlerConstPtr exceptions_handler);

  void Next(const HttpRequestConstPtr& request,
            const HttpResponsePtr& response) const override;
  void HandleRequest(const HttpRequestConstPtr& request,
                     const HttpResponsePtr& response) const override;
  void DoFilter(const HttpRequestConstPtr& request,
                const HttpResponsePtr& response) const override;

 private:
  const HttpFilterBaseConstPtr http_filter_;
  const HttpFilterContextConstPtr next_http_filter_ctx_;
  const HttpHandlerBaseConstPtr http_handler_;
  const ExceptionsHandlerConstPtr exceptions_handler_;
};

}  // namespace engine::http_server