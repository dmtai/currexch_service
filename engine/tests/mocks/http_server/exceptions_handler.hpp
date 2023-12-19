#pragma once
#include <gmock/gmock.h>
#include "src/http_server/exceptions_handler.hpp"

namespace engine::http_server {

class MockExceptionsHandler : public ExceptionsHandler {
 public:
  MOCK_METHOD(void, BracketOp,
              (const HttpRequest& request, HttpResponse& response,
               std::exception_ptr ex_ptr),
              (const));
  void operator()(const HttpRequest& request, HttpResponse& response,
                  std::exception_ptr ex_ptr) const override {
    BracketOp(request, response, ex_ptr);
  }
};

}  // namespace engine::http_server