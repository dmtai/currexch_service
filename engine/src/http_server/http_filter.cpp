#include "http_filter.hpp"
#include "exceptions_handler.hpp"

namespace engine::http_server {

void HttpFilterBase::Process(
    const HttpRequestConstPtr& request, const HttpResponsePtr& response,
    const HttpFilterContext& filter_ctx,
    const ExceptionsHandler& exceptions_handler) const {
  if (!response) {
    LOG(error, "HttpResponsePtr is nullptr");
    return;
  }
  if (!request) {
    LOG(error, "HttpRequestConstPtr is nullptr");
    return response->Close();
  }
  try {
    DoFilter(request, response, filter_ctx);
  } catch (...) {
    exceptions_handler(*request, *response, std::current_exception());
  }
}

}  // namespace engine::http_server