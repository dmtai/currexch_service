#include "http_request.hpp"
#include <memory>

namespace engine::http_server {

HttpRequestImpl::HttpRequestImpl(BeastRequest&& request) noexcept
    : request_{std::move(request)},
      uri_{boost::urls::parse_origin_form(request_.target())},
      path_{uri_->path()},
      query_{uri_->query()} {}

std::string_view HttpRequestImpl::GetUri() const { return request_.target(); }

const std::string& HttpRequestImpl::GetPath() const { return path_; }

HttpRequest::Query HttpRequestImpl::GetQuery() const {
  return boost::urls::parse_query(query_);
}

HttpMethod HttpRequestImpl::GetMethod() const {
  return ToHttpMethodEnum(request_.method());
}

const router::MatchResult& HttpRequestImpl::GetMatchResult() const {
  return match_result_;
}

const std::string& HttpRequestImpl::GetBody() const noexcept {
  return request_.body();
}

void HttpRequestImpl::SetMatchResult(router::MatchResult match_result) {
  match_result_ = std::move(match_result);
}

bool HttpRequestImpl::GetKeepAlive() const { return request_.keep_alive(); }

unsigned HttpRequestImpl::GetHttpVersion() const noexcept {
  return request_.version();
}

HttpRequestPtr MakeHttpRequest(http::request<http::string_body> request) {
  return std::make_shared<HttpRequestImpl>(std::move(request));
}

}  // namespace engine::http_server