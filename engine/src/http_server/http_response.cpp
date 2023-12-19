#include "http_response.hpp"
#include "fmt/core.h"

namespace engine::http_server {

HttpResponsePtr MakeHttpResponse(HttpSessionBasePtr session, bool keep_alive,
                                 unsigned http_version,
                                 ContentType content_type) {
  auto response =
      std::make_shared<HttpResponseImpl<HttpSessionBase>>(std::move(session));
  response->SetContentType(content_type);
  response->SetKeepAlive(keep_alive);
  response->SetStatusCode(HttpStatus::kOk);
  response->SetHttpVersion(http_version);
  return response;
}

http::response<http::string_body> MakeBeastResponse(http::status status,
                                                    unsigned http_version,
                                                    bool keep_alive,
                                                    std::string_view content,
                                                    ContentType content_type) {
  http::response<http::string_body> response{status, http_version};
  response.set(http::field::content_type, ToString(content_type));
  response.body() = content;
  response.keep_alive(keep_alive);
  response.prepare_payload();
  return response;
}

}  // namespace engine::http_server