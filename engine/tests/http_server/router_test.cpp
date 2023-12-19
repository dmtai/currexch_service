#include "src/http_server/router.hpp"
#include <gtest/gtest.h>
#include <string>
#include "../mocks/http_server/mocks.hpp"

namespace engine::http_server {

TEST(RouterTest, Find) {
  router::Router router;
  const auto http_handler = std::shared_ptr<MockHttpHandler>();
  router.Add(HttpMethod::kGet, "/article/{id}", "get_articles", http_handler);

  const auto find_result1 = *router.Find(HttpMethod::kGet, "/article/666");
  EXPECT_EQ("get_articles", find_result1.handler_name_);
  EXPECT_EQ(http_handler, find_result1.handler_);
  EXPECT_EQ("666", find_result1.match_result_["id"]);

  const auto find_result2 = router.Find(HttpMethod::kPost, "/articles");
  EXPECT_EQ(find_result2, std::nullopt);

  const auto http_handler_json = std::shared_ptr<MockHttpHandlerJson>();
  router.Add(HttpMethod::kPut, "/articles", "put_articles", http_handler_json);

  const auto find_result3 = *router.Find(HttpMethod::kPut, "/articles");
  EXPECT_EQ("put_articles", find_result3.handler_name_);
  EXPECT_EQ(http_handler_json, find_result3.handler_);
}

}  // namespace engine::http_server