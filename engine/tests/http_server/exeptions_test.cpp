#include <gtest/gtest.h>
#include "src/http_server/exceptions.hpp"

namespace engine::http_server {

TEST(HttpExceptionTest, GetHttpStatus) {
  HttpException ex{"error", HttpStatus::kOk};
  EXPECT_EQ(HttpStatus::kOk, ex.GetHttpStatus());
  EXPECT_STREQ("error", ex.what());
}

}  // namespace engine::http_server