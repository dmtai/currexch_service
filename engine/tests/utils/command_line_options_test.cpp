#include "src/utils/command_line_options.hpp"
#include <gtest/gtest.h>
#include <string>

namespace engine::utils {

TEST(CommandLineOptionsImplTest, AllOptions) {
  CommandLineOptionsImpl options{"~/path/to/config.yaml"};
  EXPECT_EQ("~/path/to/config.yaml", options.GetConfigPath());
}

TEST(ParseCommandLineTest, Help) {
  const int argc{2};
  const char* argv[] = {"~/tests/engine_tests", "--help"};
  testing::internal::CaptureStdout();
  ParseCommandLine(argc, argv);
  const auto output = testing::internal::GetCapturedStdout();
  constexpr std::string_view expected_output{
      "All options:\n  --config arg          path to config\n  --help          "
      "      help message\n\n"};
  ASSERT_EQ(expected_output, output);
}

TEST(ParseCommandLineTest, NoOptions) {
  const int argc{1};
  const char* argv[] = {"~/tests/engine_tests"};
  ASSERT_ANY_THROW({
    try {
      ParseCommandLine(argc, argv);
    } catch (const std::exception& ex) {
      EXPECT_STREQ("the option '--config' is required but missing", ex.what());
      throw;
    }
  });
}

TEST(ParseCommandLineTest, UnrecognizedOption) {
  const int argc{2};
  const char* argv[] = {"~/tests/engine_tests",
                        "--unknown=~/path/to/config.yaml"};
  ASSERT_ANY_THROW(ParseCommandLine(argc, argv));
}

TEST(ParseCommandLineTest, ConfigPath) {
  {
    const int argc{2};
    const char* argv[] = {"~/tests/engine_tests",
                          "--config=~/path/to/config.yaml"};
    const auto options = ParseCommandLine(argc, argv);
    ASSERT_NE(nullptr, options);
    ASSERT_EQ("~/path/to/config.yaml", options->GetConfigPath());
  }
}

}  // namespace engine::utils