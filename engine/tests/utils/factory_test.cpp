#include "src/utils/factory.hpp"
#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <unordered_set>

namespace engine::utils {

namespace {

class TestHanderBase {
 public:
  virtual ~TestHanderBase() = default;
};

using TestHandlerBasePtr = std::shared_ptr<TestHanderBase>;

class TestHandler : public TestHanderBase {
 public:
  TestHandler(const std::string&, int) {}
  static constexpr std::string_view kName{"test_handler"};
};

class TestHandler1 : public TestHanderBase {
 public:
  TestHandler1(const std::string&, int) {}
  static constexpr std::string_view kName{"test_handler1"};
};

class TestHandler2 : public TestHanderBase {
 public:
  TestHandler2(const std::string&, int) {}
  static constexpr std::string_view kName{"test_handler2"};
};

class TestHandlersFactory
    : public FactoryBase<TestHanderBase, const std::string&, int> {};

}  // namespace

TEST(FactoryBaseTest, Create) {
  TestHandlersFactory factory;

  {
    factory.Add<TestHandler>();
    const auto base = factory.Create("test_handler", "data", 666);
    ASSERT_NE(nullptr, base);
    const auto derived = std::dynamic_pointer_cast<TestHandler>(base);
    ASSERT_NE(nullptr, derived);
    const auto err = factory.Create("unregistred_name", "data", 666);
    ASSERT_EQ(nullptr, err);
  }

  {
    factory.Add<TestHandler1>("handler");
    const auto base = factory.Create("handler", "data", 321);
    ASSERT_NE(nullptr, base);
    const auto derived = std::dynamic_pointer_cast<TestHandler1>(base);
    ASSERT_NE(nullptr, derived);
  }
}

TEST(FactoryBaseTest, ForEach) {
  TestHandlersFactory factory;
  factory.Add<TestHandler>();
  factory.Add<TestHandler1>();
  factory.Add<TestHandler2>();

  std::unordered_set<TestHandlerBasePtr> handlers;
  const auto for_each = [&](const auto& name, const auto& cb) {
    const auto base = cb("test", 123);
    ASSERT_NE(nullptr, base);
    if (name == TestHandler::kName) {
      const auto derived = std::dynamic_pointer_cast<TestHandler>(base);
      ASSERT_NE(nullptr, derived);
      handlers.insert(base);
      return;
    } else if (name == TestHandler1::kName) {
      const auto derived = std::dynamic_pointer_cast<TestHandler1>(base);
      ASSERT_NE(nullptr, derived);
      handlers.insert(base);
      return;
    } else if (name == TestHandler2::kName) {
      const auto derived = std::dynamic_pointer_cast<TestHandler2>(base);
      ASSERT_NE(nullptr, derived);
      handlers.insert(base);
      return;
    } else {
      FAIL();
    }
  };
  factory.ForEach(for_each);

  ASSERT_EQ(3, handlers.size());
}

}  // namespace engine::utils