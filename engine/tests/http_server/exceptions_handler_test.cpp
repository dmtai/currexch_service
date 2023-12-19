#include "src/http_server/exceptions_handler.hpp"
#include <gtest/gtest.h>
#include <string>
#include "../mocks/http_server/mocks.hpp"

namespace engine::http_server {
using namespace ::testing;

TEST(ExceptionsHandlerImplTest, BracketOperator) {
  NiceMock<MockHttpRequest> http_request;
  NiceMock<MockHttpResponse> http_response;
  ExceptionsHandlerImpl exceptions_handler;

  const auto http_ex_ptr = std::make_exception_ptr(
      HttpException{"HTTP error", HttpStatus::kBadRequest});
  EXPECT_CALL(http_response, SetStatusCode(HttpStatus::kBadRequest)).Times(1);
  EXPECT_CALL(http_response, Send("HTTP error")).Times(1);
  exceptions_handler(http_request, http_response, http_ex_ptr);

  const auto ex_ptr = std::make_exception_ptr(std::runtime_error{"Error"});
  EXPECT_CALL(http_response, SetStatusCode(HttpStatus::kInternalServerError))
      .Times(1);
  EXPECT_CALL(http_response, Send).Times(1);
  exceptions_handler(http_request, http_response, ex_ptr);

  const auto unknown_ex_ptr =
      std::make_exception_ptr(std::string{"Unknown error"});
  EXPECT_CALL(http_response, Close()).Times(1);
  exceptions_handler(http_request, http_response, unknown_ex_ptr);
}

}  // namespace engine::http_server