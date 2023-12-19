#include "src/http_server/http_request.hpp"
#include <gtest/gtest.h>
#include <string>
#include "../mocks/http_server/mocks.hpp"

namespace engine::http_server {
using namespace ::testing;

namespace {

class HttpRequestImplTest : public testing::Test {
 protected:
  void SetUp() override {
    beast_request_.target("/articles");
    beast_request_.method(http::verb::post);
    beast_request_.version(1);
    beast_request_.keep_alive(true);
    beast_request_.body() = "Request body";
  }

  http::request<http::string_body> beast_request_;
};

}  // namespace

TEST_F(HttpRequestImplTest, GetUri) {
  HttpRequestImpl http_request{std::move(beast_request_)};
  EXPECT_EQ("/articles", http_request.GetUri());
}

TEST_F(HttpRequestImplTest, GetMethod) {
  HttpRequestImpl http_request{std::move(beast_request_)};
  EXPECT_EQ(HttpMethod::kPost, http_request.GetMethod());
}

TEST_F(HttpRequestImplTest, GetBody) {
  HttpRequestImpl http_request{std::move(beast_request_)};
  EXPECT_EQ("Request body", http_request.GetBody());
}

TEST_F(HttpRequestImplTest, GetKeepAlive) {
  HttpRequestImpl http_request{std::move(beast_request_)};
  EXPECT_EQ(true, http_request.GetKeepAlive());
}

TEST_F(HttpRequestImplTest, GetHttpVersion) {
  HttpRequestImpl http_request{std::move(beast_request_)};
  EXPECT_EQ(1, http_request.GetKeepAlive());
}

}  // namespace engine::http_server