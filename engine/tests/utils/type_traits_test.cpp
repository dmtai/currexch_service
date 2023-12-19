#include "src/utils/type_traits.hpp"
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace engine::utils {

TEST(IsStringTypeTest, IsStringType) {
  EXPECT_TRUE(utils::is_string_type_v<std::string>);
  EXPECT_TRUE(utils::is_string_type_v<std::string_view>);
  EXPECT_FALSE(utils::is_string_type_v<int>);
  EXPECT_FALSE(utils::is_string_type_v<std::vector<std::string>>);
}

}  // namespace engine::utils