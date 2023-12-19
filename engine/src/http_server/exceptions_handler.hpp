#pragma once

#include <memory>
#include "exceptions.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

namespace engine::http_server {

class ExceptionsHandler {
 public:
  virtual ~ExceptionsHandler() = default;
  virtual void operator()(const HttpRequest& request, HttpResponse& response,
                          std::exception_ptr) const = 0;
};

class ExceptionsHandlerImpl : public ExceptionsHandler {
 public:
  void operator()(const HttpRequest& request, HttpResponse& response,
                  std::exception_ptr ex_ptr) const override;
};

void HandleException(const HttpRequest& request, HttpResponse& response,
                     std::exception_ptr ex_ptr);

using ExceptionsHandlerConstPtr = std::shared_ptr<const ExceptionsHandler>;

}  // namespace engine::http_server