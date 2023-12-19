#include "http_filter_context.hpp"

namespace engine::http_server {

HttpFilterContextImpl::HttpFilterContextImpl(
    HttpFilterBaseConstPtr http_filter,
    HttpFilterContextConstPtr next_http_filter_ctx,
    HttpHandlerBaseConstPtr http_handler_base,
    ExceptionsHandlerConstPtr exceptions_handler)
    : http_filter_{std::move(http_filter)},
      next_http_filter_ctx_{std::move(next_http_filter_ctx)},
      http_handler_{std::move(http_handler_base)},
      exceptions_handler_{std::move(exceptions_handler)} {}

void HttpFilterContextImpl::Next(const HttpRequestConstPtr& request,
                                 const HttpResponsePtr& response) const {
  if (next_http_filter_ctx_) {
    next_http_filter_ctx_->DoFilter(request, response);
  } else if (http_handler_) {
    http_handler_->Process(request, response, *exceptions_handler_);
  } else {
    LOG(critical, "There is no next filter and handler");
    if (response) {
      response->Close();
    } else {
      LOG(error, "HttpResponsePtr is nullptr");
    }
  }
}

void HttpFilterContextImpl::HandleRequest(
    const HttpRequestConstPtr& request, const HttpResponsePtr& response) const {
  http_handler_->Process(std::move(request), std::move(response),
                         *exceptions_handler_);
}

void HttpFilterContextImpl::DoFilter(const HttpRequestConstPtr& request,
                                     const HttpResponsePtr& response) const {
  http_filter_->Process(request, response, *this, *exceptions_handler_);
}

}  // namespace engine::http_server