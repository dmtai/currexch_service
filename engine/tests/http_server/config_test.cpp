#include "src/utils/config.hpp"
#include <gtest/gtest.h>
#include "src/http_server/config.hpp"

namespace engine::http_server {

namespace {

class ConfigTest : public testing::Test {
 protected:
  void SetUp() override {
    config_["general"]["address"] = "127.0.0.1";
    config_["general"]["port"] = "6666";
    config_["general"]["session_timeout"] = "332211";
    config_["general"]["log_path"] = "~/path/to/log.log";
    config_["general"]["log_level"] = "info";

    YamlConfig http_handlers;

    YamlConfig http_handler1;
    http_handler1["path"] = "/articles";
    http_handler1["method"] = "get";

    YamlConfig http_filter;
    http_filter["setting"] = "data";

    YamlConfig http_handler2;
    http_handler2["path"] = "/article/{id}";
    http_handler2["method"] = "post";
    http_handler2["filters"]["http_filter1"] = http_filter;

    http_handlers["http_handler1"] = http_handler1;
    http_handlers["http_handler2"] = http_handler2;

    config_["http-handlers"] = http_handlers;

    YamlConfig component;
    component["connect_info"] = "connectinfo";
    YamlConfig components;
    components["pg_client"] = component;
    config_["components"] = components;
  }

  YamlConfig config_;
};

}  // namespace

TEST_F(ConfigTest, ParseConfig) {
  auto config = config::ParseConfig(config_);

  EXPECT_EQ("127.0.0.1", config.general_.address_);
  EXPECT_EQ(6666, config.general_.port_);
  EXPECT_EQ(332211, config.general_.session_timeout_);
  EXPECT_EQ("~/path/to/log.log", config.general_.log_path_);
  EXPECT_EQ(logger::LoggerBase::Level::info, config.general_.log_level_);

  ASSERT_EQ(2, config.http_handlers_.size());
  EXPECT_EQ("/articles", config.http_handlers_["http_handler1"].path_);
  EXPECT_EQ("get", config.http_handlers_["http_handler1"].method_);
  EXPECT_EQ(0, config.http_handlers_["http_handler1"].filters_.size());

  EXPECT_EQ("/article/{id}", config.http_handlers_["http_handler2"].path_);
  EXPECT_EQ("post", config.http_handlers_["http_handler2"].method_);
  ASSERT_EQ(1, config.http_handlers_["http_handler2"].filters_.size());

  auto filter = config.http_handlers_["http_handler2"].filters_[0];
  EXPECT_EQ("http_filter1", filter.first);
  EXPECT_EQ("data", filter.second["setting"].as<std::string>());
  EXPECT_EQ("connectinfo",
            config.components_["pg_client"]["connect_info"].as<std::string>());
}

}  // namespace engine::http_server