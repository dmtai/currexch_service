#include "src/http_server/http_filter_ctxs_map.hpp"
#include <gtest/gtest.h>
#include <string>
#include "../mocks/http_server/mocks.hpp"

namespace engine::http_server {
using namespace ::testing;

TEST(HttpFilterContextsMapImplTest, AddAndHasFilters) {
  HttpFilterContextsMapImpl http_filter_contexts_map;
  const auto filter_context = std::make_shared<MockHttpFilterContext>();
  http_filter_contexts_map.AddFront("get_articles", filter_context);
  ASSERT_TRUE(http_filter_contexts_map.HasFilters("get_articles"));
  ASSERT_FALSE(http_filter_contexts_map.HasFilters("post_article"));
}

TEST(HttpFilterContextsMapImplTest, DoFiltersAndHandler) {
  HttpFilterContextsMapImpl http_filter_contexts_map;
  const auto filter_context = std::make_shared<MockHttpFilterContext>();
  http_filter_contexts_map.AddFront("get_articles", filter_context);

  const auto http_request = std::make_shared<const MockHttpRequest>();
  const auto http_response = std::make_shared<MockHttpResponse>();

  EXPECT_CALL(*filter_context, DoFilter(_, _)).Times(1);
  http_filter_contexts_map.DoFiltersAndHandler("get_articles", http_request,
                                               http_response);

  EXPECT_CALL(*filter_context, DoFilter(_, _)).Times(0);
  http_filter_contexts_map.DoFiltersAndHandler("update_articles", http_request,
                                               http_response);
}

}  // namespace engine::http_server