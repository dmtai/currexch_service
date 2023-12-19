#include "src/utils/logger.hpp"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <thread>
#include <unordered_set>

namespace engine::logger {

TEST(StdoutLoggerTest, Log) {
  StdoutLogger logger{"logger"};
  EXPECT_EQ(Logger::trace, logger.GetLevel());

  logger.SetLevel(LoggerBase::Level::info);
  EXPECT_EQ(Logger::info, logger.GetLevel());

  logger.SetLogFormat("%v");

  testing::internal::CaptureStdout();
  logger.Log(::spdlog::source_loc{__FILE__, __LINE__, __func__}, Logger::info,
             "Logger test");
  logger.Flush();
  const auto output = testing::internal::GetCapturedStdout();
  ASSERT_EQ("Logger test\n", output);
}

}  // namespace engine::logger