#include "src/http_server/http_response.hpp"
#include <gtest/gtest.h>
#include <string>
#include "../mocks/http_server/mocks.hpp"

namespace engine::http_server {
using namespace ::testing;

TEST(HttpResponseImplTest, Send) {
  const auto session = std::make_shared<MockHttpSession>();
  HttpResponseImpl<MockHttpSession> http_response{session};
  http_response.SetContentType(ContentType::kApplicationJson);
  http_response.SetKeepAlive(true);
  http_response.SetStatusCode(HttpStatus::kOk);
  http_response.SetHttpVersion(1);

  EXPECT_CALL(*session, Write(Matcher<http::response<http::string_body>>(_)))
      .Times(1);
  http_response.Send("Response");
}

}  // namespace engine::http_server