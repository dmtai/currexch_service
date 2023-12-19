#pragma once

#include <boost/url.hpp>
#include <memory>
#include "http_method.hpp"
#include "router.hpp"
#include "utils/json.hpp"
#include "utils/net.hpp"

namespace engine::http_server {

class HttpRequest {
 public:
  using Query = boost::system::result<boost::urls::params_encoded_view>;

  virtual ~HttpRequest() = default;

  virtual std::string_view GetUri() const = 0;
  virtual const std::string& GetPath() const = 0;
  virtual Query GetQuery() const = 0;
  virtual HttpMethod GetMethod() const = 0;
  virtual const router::MatchResult& GetMatchResult() const = 0;
  virtual const std::string& GetBody() const noexcept = 0;
  virtual bool GetKeepAlive() const = 0;
  virtual unsigned GetHttpVersion() const noexcept = 0;
  virtual void SetMatchResult(router::MatchResult match_result) = 0;
};

class HttpRequestImpl : public HttpRequest {
 public:
  using BeastRequest = http::request<http::string_body>;
  using Uri = boost::system::result<boost::urls::url_view>;

  explicit HttpRequestImpl(BeastRequest&& request) noexcept;
  std::string_view GetUri() const override;
  const std::string& GetPath() const override;
  HttpRequest::Query GetQuery() const override;
  HttpMethod GetMethod() const override;
  const router::MatchResult& GetMatchResult() const override;
  const std::string& GetBody() const noexcept override;
  bool GetKeepAlive() const override;
  unsigned GetHttpVersion() const noexcept override;
  void SetMatchResult(router::MatchResult match_result) override;

 private:
  router::MatchResult match_result_;
  BeastRequest request_;
  Uri uri_;
  const std::string path_;
  const std::string query_;
};

using HttpRequestPtr = std::shared_ptr<HttpRequest>;
using HttpRequestConstPtr = std::shared_ptr<const HttpRequest>;

HttpRequestPtr MakeHttpRequest(http::request<http::string_body> request);

}  // namespace engine::http_server