#include "src/utils/config.hpp"
#include <gtest/gtest.h>

namespace engine::config {

namespace {

class RetrieveValidOrEmptyConfigTest : public testing::Test {
 protected:
  void SetUp() override {
    config_["users"] = YAML::Load("{name: John, city: New-York}");
  }

  YamlConfig config_;
};

}  // namespace

TEST_F(RetrieveValidOrEmptyConfigTest, RetrieveValidOrEmptyConfig) {
  const auto valid = RetrieveValidOrEmptyConfig("users", config_);
  EXPECT_EQ(config_["users"], valid);

  const auto empty = RetrieveValidOrEmptyConfig("unknown", config_);
  EXPECT_TRUE(empty.IsNull());

  const auto empty_config_from_invalid_node =
      RetrieveValidOrEmptyConfig("unknown", config_["invalid_node"]);
  EXPECT_TRUE(empty_config_from_invalid_node.IsNull());
}

}  // namespace engine::config