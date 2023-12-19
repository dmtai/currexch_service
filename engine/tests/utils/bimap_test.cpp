#include "src/utils/bimap.hpp"
#include <gtest/gtest.h>
#include <string>

namespace engine::utils {

TEST(BimapTest, CaseInsensitiveUnorderedBiMap) {
  using TestCaseInsensitiveUnorderedBiMap =
      CaseInsensitiveUnorderedBiMap<std::string, int>;

  const auto bimap = MakeBimap<TestCaseInsensitiveUnorderedBiMap>(
      {{"one", 1}, {"TwO", 2}, {"THREE", 3}});

  const auto it = bimap.left.find("ONe");
  ASSERT_TRUE(it != bimap.left.end());
  ASSERT_EQ(it->second, 1);

  const auto it1 = bimap.left.find("TwO");
  ASSERT_TRUE(it1 != bimap.left.end());
  ASSERT_EQ(it1->second, 2);

  const auto it2 = bimap.left.find("THREEE");
  ASSERT_TRUE(it2 == bimap.left.end());

  const auto it3 = bimap.right.find(3);
  ASSERT_TRUE(it3 != bimap.right.end());
  ASSERT_EQ(it3->second, "THREE");
}

}  // namespace engine::utils