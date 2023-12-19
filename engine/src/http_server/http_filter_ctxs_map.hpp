#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include "http_filter_context.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

namespace engine::http_server {

class HttpFilterContextsMap {
 public:
  using HandlerName = std::string;

  virtual ~HttpFilterContextsMap() = default;

  virtual void AddFront(const HandlerName& name,
                        HttpFilterContextConstPtr http_filter) = 0;
  [[nodiscard]] virtual bool HasFilters(
      const HandlerName& handler_name) const = 0;
  virtual void DoFiltersAndHandler(const HandlerName& name,
                                   const HttpRequestConstPtr& request,
                                   const HttpResponsePtr& response) const = 0;
};

class HttpFilterContextsMapImpl : public HttpFilterContextsMap {
 public:
  using FilterCtxsList = std::list<HttpFilterContextConstPtr>;
  using FilterCtxsByHandlerMap =
      std::unordered_map<HandlerName, FilterCtxsList>;

  HttpFilterContextsMapImpl() = default;
  HttpFilterContextsMapImpl(const HttpFilterContextsMapImpl&) = delete;
  HttpFilterContextsMapImpl& operator=(const HttpFilterContextsMapImpl&) =
      delete;
  HttpFilterContextsMapImpl(HttpFilterContextsMapImpl&&) = default;
  HttpFilterContextsMapImpl& operator=(HttpFilterContextsMapImpl&&) = default;

  void AddFront(const HandlerName& name,
                HttpFilterContextConstPtr http_filter) override;
  [[nodiscard]] bool HasFilters(const HandlerName& handler_name) const override;
  void DoFiltersAndHandler(const HandlerName& name,
                           const HttpRequestConstPtr& request,
                           const HttpResponsePtr& response) const override;

 private:
  FilterCtxsByHandlerMap filters_by_handler_;
};

}  // namespace engine::http_server