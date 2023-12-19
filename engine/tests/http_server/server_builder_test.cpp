#include "src/http_server/server_builder.hpp"
#include <gtest/gtest.h>
#include <string>
#include "../mocks/http_server/mocks.hpp"
#include "src/utils/config.hpp"

namespace engine::http_server {

namespace {

class Handler1 : public HttpHandlerJson {
 public:
  static constexpr std::string_view kName{"get_articles"};

  Handler1(const YamlConfig& config, components::Components& components) {}

  void HandleRequestJson(const HttpRequestConstPtr& request,
                         const HttpResponsePtr& response,
                         const Json& body_json) const override {}
};

class Handler2 : public HttpHandlerJson {
 public:
  static constexpr std::string_view kName{"post_article"};

  Handler2(const YamlConfig& config, components::Components& components) {}

  void HandleRequestJson(const HttpRequestConstPtr& request,
                         const HttpResponsePtr& response,
                         const Json& body_json) const override {}
};

class Filter1 : public engine::http_server::HttpFilterBase {
 public:
  static constexpr std::string_view kName{"auth_checker"};

  Filter1(engine::YamlConfig config, engine::components::Components&) {}

  void DoFilter(
      const engine::http_server::HttpRequestConstPtr& request,
      const engine::http_server::HttpResponsePtr& response,
      const engine::http_server::HttpFilterContext& ctx) const override {}
};

class ServerBuilderTest : public testing::Test {
 protected:
  void SetUp() override {
    YamlConfig http_handlers;

    YamlConfig http_handler1;
    http_handler1["path"] = "/articles";
    http_handler1["method"] = "get";

    YamlConfig http_filter;
    http_filter["setting"] = "data";

    YamlConfig http_handler2;
    http_handler2["path"] = "/article/{id}";
    http_handler2["method"] = "post";
    http_handler2["filters"]["auth_checker"] = http_filter;

    http_handlers["get_articles"] = http_handler1;
    http_handlers["post_article"] = http_handler2;

    config_["http-handlers"] = http_handlers;

    exceptions_handler_ = std::make_shared<MockExceptionsHandler>();
  }

  YamlConfig config_;
  std::shared_ptr<MockExceptionsHandler> exceptions_handler_;
};

}  // namespace

TEST_F(ServerBuilderTest, MakeRouterAndHttpFilterCtxsMap) {
  HttpHandlersFactory handlers_factory;
  handlers_factory.Add<Handler1>();
  handlers_factory.Add<Handler2>();
  HttpFiltersFactory filters_factory;
  filters_factory.Add<Filter1>();
  const auto components_factory =
      std::make_shared<components::ComponentsFactory>();

  components::Components components{YamlConfig{}, components_factory};

  auto bad_config1 = config::ParseConfig(config_);
  bad_config1.http_handlers_.insert({"unknown_handler", {}});
  EXPECT_ANY_THROW({
    MakeRouterAndHttpFilterCtxsMap(handlers_factory, filters_factory,
                                   bad_config1, components,
                                   exceptions_handler_);
  });

  auto bad_config2 = config::ParseConfig(config_);
  bad_config2.http_handlers_["post_article"].method_ = "unknown_http_method";
  EXPECT_ANY_THROW({
    MakeRouterAndHttpFilterCtxsMap(handlers_factory, filters_factory,
                                   bad_config2, components,
                                   exceptions_handler_);
  });

  const auto config = config::ParseConfig(config_);
  EXPECT_NO_THROW({
    const auto router_and_filters =
        MakeRouterAndHttpFilterCtxsMap(handlers_factory, filters_factory,
                                       config, components, exceptions_handler_);
    const auto get_articles_find_result =
        router_and_filters.first->Find(HttpMethod::kGet, "/articles");
    EXPECT_NE(std::nullopt, get_articles_find_result);
    const auto post_article_find_result =
        router_and_filters.first->Find(HttpMethod::kPost, "/article/666");
    EXPECT_NE(std::nullopt, post_article_find_result);
    EXPECT_TRUE(router_and_filters.second.HasFilters("post_article"));
    EXPECT_FALSE(router_and_filters.second.HasFilters("unknown_filter"));
  });
}

}  // namespace engine::http_server