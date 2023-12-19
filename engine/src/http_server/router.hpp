#pragma once

#include <boost/url.hpp>
#include <memory>
#include <optional>
#include <utility>
#include "http_handler.hpp"
#include "http_method.hpp"
#include "third_party/router/include/router.hpp"

namespace engine::http_server::router {

namespace urls = boost::urls;
using MatchResult = urls::matches;

class Router {
 public:
  using HandlerName = std::string;
  using Handler = std::pair<HandlerName, HttpHandlerBaseConstPtr>;
  using UrlsRouter = urls::router<Handler>;
  using RouterByMethodMap = std::unordered_map<HttpMethod, UrlsRouter>;

  struct FindResult {
    const HandlerName& handler_name_;
    const HttpHandlerBaseConstPtr& handler_;
    MatchResult match_result_;
  };
  using FindResultOpt = std::optional<FindResult>;

  Router() = default;
  Router(const Router&) = delete;
  Router& operator=(const Router&) = delete;
  Router(Router&&) = delete;
  Router& operator=(Router&&) = delete;

  FindResultOpt Find(HttpMethod http_method, std::string_view uri) const;

  void Add(HttpMethod http_method, std::string_view uri,
           HandlerName handler_name, HttpHandlerBaseConstPtr handler);

 private:
  RouterByMethodMap routes_;
};

using RouterPtr = std::shared_ptr<Router>;

}  // namespace engine::http_server::router