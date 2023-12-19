#include "http_filter_ctxs_map.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "http_request.hpp"
#include "http_response.hpp"

namespace engine::http_server {

void HttpFilterContextsMapImpl::AddFront(
    const HandlerName& name, HttpFilterContextConstPtr http_filter_ctx) {
  filters_by_handler_[name].push_front(std::move(http_filter_ctx));
}

bool HttpFilterContextsMapImpl::HasFilters(const HandlerName& name) const {
  auto filters = filters_by_handler_.find(name);
  return filters != filters_by_handler_.end() && !filters->second.empty();
}

void HttpFilterContextsMapImpl::DoFiltersAndHandler(
    const HandlerName& name, const HttpRequestConstPtr& request,
    const HttpResponsePtr& response) const {
  auto filters = filters_by_handler_.find(name);
  if (filters == filters_by_handler_.end() || filters->second.empty()) {
    return;
  }
  filters->second.front()->DoFilter(request, response);
}

}  // namespace engine::http_server