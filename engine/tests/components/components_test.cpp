#include "src/components/components.hpp"
#include <gtest/gtest.h>
#include <string>
#include "../mocks/http_server/mocks.hpp"

namespace engine::components {
namespace {

class Component1 : public ComponentBase {
 public:
  static constexpr std::string_view kName{"component1"};

  Component1(const YamlConfig& config, Components& components) {
    data_ = config["setting"].as<std::string>();
  }

  const std::string& GetData() const noexcept { return data_; }

 private:
  std::string data_;
};

class Component2 : public ComponentBase {
 public:
  static constexpr std::string_view kName{"component2"};

  Component2(const YamlConfig& config, Components& components) {
    data_ = config["setting"].as<std::string>();
  }

  const std::string& GetData() const noexcept { return data_; }

 private:
  std::string data_;
};

class Component3 : public ComponentBase {
 public:
  static constexpr std::string_view kName{"component3"};

  Component3(const YamlConfig& config, Components& components) {
    data_ = config["setting"].as<std::string>();
  }

  const std::string& GetData() const noexcept { return data_; }

 private:
  std::string data_;
};

class ComponentsTest : public testing::Test {
 protected:
  void SetUp() override {
    config_["component1"]["setting"] = "data";
    config_["component2"]["setting"] = "data";
    config_["component3"]["setting"] = "data";

    components_factory_ = std::make_shared<ComponentsFactory>();
    components_factory_->Add<Component1>();
    components_factory_->Add<Component2>();
    components_factory_->Add<Component3>();
  }

  std::shared_ptr<ComponentsFactory> components_factory_;
  YamlConfig config_;
};

}  // namespace

TEST_F(ComponentsTest, Add) {
  Components components{config_, components_factory_};
  EXPECT_NO_THROW({
    components.Add(Component1::kName, std::make_shared<Component1>(
                                          config_["component1"], components));
  });

  EXPECT_ANY_THROW({ components.Add(Component2::kName, nullptr); });
}

TEST_F(ComponentsTest, Contains) {
  Components components{config_, components_factory_};
  components.Add(Component1::kName, std::make_shared<Component1>(
                                        config_["component1"], components));

  EXPECT_TRUE(components.Contains("component1"));
  EXPECT_FALSE(components.Contains("unknown_component"));
}

TEST_F(ComponentsTest, Get) {
  Components components{config_, components_factory_};
  components.Add(Component1::kName, std::make_shared<Component1>(
                                        config_["component1"], components));

  EXPECT_NO_THROW({
    const auto component1 = components.Get<Component1>();
    EXPECT_EQ("data", component1->GetData());
  });

  EXPECT_NO_THROW({
    const auto component2 = components.Get<Component2>();
    EXPECT_EQ("data", component2->GetData());
    EXPECT_TRUE(components.Contains("component2"));
  });

  EXPECT_ANY_THROW(
      { const auto component1 = components.Get<Component2>("component1"); });

  EXPECT_ANY_THROW(
      { const auto component1 = components.Get<Component1>("component4"); });

  EXPECT_ANY_THROW(
      { const auto component1 = components.Get<Component2>("component3"); });
}

TEST_F(ComponentsTest, ForEach) {
  Components components{config_, components_factory_};
  components.Add(Component1::kName, std::make_shared<Component1>(
                                        config_["component1"], components));
  components.Add(Component2::kName, std::make_shared<Component2>(
                                        config_["component2"], components));
  components.Add(Component3::kName, std::make_shared<Component3>(
                                        config_["component3"], components));

  const auto for_each = [&](const auto& component_name, const auto& component) {
    EXPECT_NO_THROW({
      if (const auto res = std::dynamic_pointer_cast<Component1>(component)) {
        EXPECT_EQ("component1", res->kName);
        EXPECT_EQ("component1", component_name);
      } else if (const auto res =
                     std::dynamic_pointer_cast<Component2>(component)) {
        EXPECT_EQ("component2", res->kName);
        EXPECT_EQ("component2", component_name);
      } else if (const auto res =
                     std::dynamic_pointer_cast<Component3>(component)) {
        EXPECT_EQ("component3", res->kName);
        EXPECT_EQ("component3", component_name);
      } else {
        throw std::runtime_error{"Unkonwn component"};
      }
    });
  };

  components.ForEach(for_each);
}

TEST_F(ComponentsTest, ForEachComponentOfType) {
  Components components{config_, components_factory_};
  components.Add(Component1::kName, std::make_shared<Component1>(
                                        config_["component1"], components));
  components.Add(Component2::kName, std::make_shared<Component2>(
                                        config_["component2"], components));
  components.Add(Component3::kName, std::make_shared<Component3>(
                                        config_["component3"], components));

  const auto for_each = [&](const auto& component) {
    EXPECT_EQ("component1", component.kName);
  };

  components.ForEachComponentOfType<Component1>(for_each);
}

}  // namespace engine::components