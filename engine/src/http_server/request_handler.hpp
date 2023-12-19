#pragma once

#include "http_filter_ctxs_map.hpp"
#include "http_request.hpp"
#include "http_response.hpp"
#include "http_session.hpp"
#include "server_context.hpp"
#include "utils/net.hpp"

namespace engine::http_server {

class RequestHandler {
 public:
  explicit RequestHandler(const ServerContext& server_context)
      : server_context_{server_context} {};

  template <typename Body, typename Allocator>
  void operator()(
      http::request<Body, http::basic_fields<Allocator>>&& beast_request,
      HttpSessionBasePtr session) const {
    auto request = MakeHttpRequest(beast_request);
    auto response = MakeHttpResponse(
        std::move(session), request->GetKeepAlive(), request->GetHttpVersion());

    const auto& router = server_context_.GetRouter();
    const auto find_result =
        router.Find(request->GetMethod(), request->GetPath());
    if (!find_result) {
      LOG(debug, "HTTP handler for \"{}\" isn't registered",
          request->GetPath());
      response->SetStatusCode(HttpStatus::kInternalServerError);
      return response->Send();
    }
    request->SetMatchResult(find_result->match_result_);

    const auto& exceptions_handler = server_context_.GetExceptionsHandler();
    const auto& http_filter_contexts_map =
        server_context_.GetHttpFilterContextsMap();
    if (!http_filter_contexts_map.HasFilters(find_result->handler_name_)) {
      return find_result->handler_->Process(request, response,
                                            exceptions_handler);
    }
    http_filter_contexts_map.DoFiltersAndHandler(find_result->handler_name_,
                                                 request, response);
  }

 private:
  const ServerContext& server_context_;
};

}  // namespace engine::http_server