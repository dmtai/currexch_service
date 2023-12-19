#include "src/http_server/http_filter_context.hpp"
#include <gtest/gtest.h>
#include <string>
#include "../mocks/http_server/mocks.hpp"

namespace engine::http_server {
using namespace ::testing;

namespace {

class HttpFilterContextImplTest : public testing::Test {
 protected:
  void SetUp() override {
    http_filter_ = std::make_shared<MockHttpFilter>();
    http_filter_context_ = std::make_shared<MockHttpFilterContext>();
    http_handler_ = std::make_shared<MockHttpHandler>();
    exceptions_handler_ = std::make_shared<MockExceptionsHandler>();
    http_request_ = std::make_shared<MockHttpRequest>();
    http_response_ = std::make_shared<MockHttpResponse>();
  }

  std::shared_ptr<MockHttpFilter> http_filter_;
  std::shared_ptr<MockHttpFilterContext> http_filter_context_;
  std::shared_ptr<MockHttpHandler> http_handler_;
  std::shared_ptr<MockExceptionsHandler> exceptions_handler_;
  std::shared_ptr<MockHttpRequest> http_request_;
  std::shared_ptr<MockHttpResponse> http_response_;
};

}  // namespace

TEST_F(HttpFilterContextImplTest, Next) {
  {
    HttpFilterContextImpl http_filter_ctx{http_filter_, http_filter_context_,
                                          http_handler_, exceptions_handler_};
    EXPECT_CALL(*http_filter_context_, DoFilter(_, _)).Times(1);
    http_filter_ctx.Next(http_request_, http_response_);
  }
  {
    HttpFilterContextImpl http_filter_ctx{http_filter_, nullptr, http_handler_,
                                          exceptions_handler_};
    EXPECT_CALL(*http_handler_, Process(_, _, _)).Times(1);
    http_filter_ctx.Next(http_request_, http_response_);
  }
  {
    HttpFilterContextImpl http_filter_ctx{http_filter_, nullptr, nullptr,
                                          exceptions_handler_};
    EXPECT_CALL(*http_response_, Close()).Times(1);
    http_filter_ctx.Next(http_request_, http_response_);
  }
  {
    HttpFilterContextImpl http_filter_ctx{http_filter_, nullptr, nullptr,
                                          exceptions_handler_};
    http_filter_ctx.Next(http_request_, nullptr);
  }
}

TEST_F(HttpFilterContextImplTest, HandleRequest) {
  HttpFilterContextImpl http_filter_ctx{http_filter_, http_filter_context_,
                                        http_handler_, exceptions_handler_};
  EXPECT_CALL(*http_handler_, Process(_, _, _)).Times(1);
  http_filter_ctx.HandleRequest(http_request_, http_response_);
}

TEST_F(HttpFilterContextImplTest, DoFilter) {
  HttpFilterContextImpl http_filter_ctx{http_filter_, http_filter_context_,
                                        http_handler_, exceptions_handler_};
  EXPECT_CALL(*http_filter_, Process(_, _, _, _)).Times(1);
  http_filter_ctx.DoFilter(http_request_, http_response_);
}

}  // namespace engine::http_server