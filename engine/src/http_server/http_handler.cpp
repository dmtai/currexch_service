#include "http_handler.hpp"
#include "exceptions_handler.hpp"

namespace engine::http_server {

void HttpHandlerBase::Process(
    const HttpRequestConstPtr& request, const HttpResponsePtr& response,
    const ExceptionsHandler& exceptions_handler) const {
  if (!response) {
    LOG(error, "HttpResponsePtr is nullptr");
    return;
  }
  if (!request) {
    LOG(error, "HttpRequestConstPtr is nullptr, connection closed");
    return response->Close();
  }
  try {
    HandleRequest(request, response);
  } catch (...) {
    exceptions_handler(*request, *response, std::current_exception());
  }
}

void HttpHandlerJson::HandleRequest(const HttpRequestConstPtr& request,
                                    const HttpResponsePtr& response) const {
  const auto body = request->GetBody();
  HandleRequestJson(request, response,
                    body.empty() ? Json{} : Json::parse(std::move(body)));
}

}  // namespace engine::http_server