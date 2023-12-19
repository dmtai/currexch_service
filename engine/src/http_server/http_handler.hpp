#pragma once

#include <memory>
#include "utils/json.hpp"

namespace engine::http_server {

using HttpRequestConstPtr = std::shared_ptr<const class HttpRequest>;
using HttpResponsePtr = std::shared_ptr<class HttpResponse>;
class ExceptionsHandler;

class HttpHandlerBase {
 public:
  virtual ~HttpHandlerBase() = default;
  virtual void HandleRequest(const HttpRequestConstPtr& request,
                             const HttpResponsePtr& response) const = 0;
  virtual void Process(const HttpRequestConstPtr& request,
                       const HttpResponsePtr& response,
                       const ExceptionsHandler& exceptions_handler) const;
};

class HttpHandlerJson : public HttpHandlerBase {
 public:
  void HandleRequest(const HttpRequestConstPtr& request,
                     const HttpResponsePtr& response) const override;
  virtual void HandleRequestJson(const HttpRequestConstPtr& request,
                                 const HttpResponsePtr& response,
                                 const Json& body_json) const = 0;
};

using HttpHandlerBasePtr = std::shared_ptr<HttpHandlerBase>;
using HttpHandlerBaseConstPtr = std::shared_ptr<const HttpHandlerBase>;

}  // namespace engine::http_server