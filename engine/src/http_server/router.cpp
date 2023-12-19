#include "router.hpp"

namespace engine::http_server::router {

Router::FindResultOpt Router::Find(HttpMethod http_method,
                                   std::string_view uri) const {
  const auto router = routes_.find(http_method);
  if (router == routes_.end()) {
    return std::nullopt;
  }
  const auto path = urls::parse_path(uri);
  if (!path) {
    return std::nullopt;
  }
  MatchResult match_result;
  const auto handler_pair = router->second.find(*path, match_result);
  if (!handler_pair) {
    return std::nullopt;
  }
  return std::make_optional<FindResult>(
      {handler_pair->first, handler_pair->second, std::move(match_result)});
}

void Router::Add(HttpMethod http_method, std::string_view uri,
                 HandlerName handler_name, HttpHandlerBaseConstPtr handler) {
  routes_[http_method].insert(
      uri, std::make_pair(std::move(handler_name), std::move(handler)));
}

}  // namespace engine::http_server::router