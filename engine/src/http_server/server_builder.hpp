#pragma once

#include <memory>
#include <string>
#include "components/factory.hpp"
#include "factories.hpp"
#include "server.hpp"
#include "utils/command_line_options.hpp"

namespace engine::http_server {

class ServerBuilder {
 public:
  ServerBuilder& SetConfig(config::ConfigConstPtr config);
  ServerPtr Build();

  template <typename T>
  ServerBuilder& AddHandler() {
    http_handlers_factory_.Add<T>();
    return *this;
  }

  template <typename T>
  ServerBuilder& AddFilter() {
    http_filters_factory_.Add<T>();
    return *this;
  }

  template <typename T>
  ServerBuilder& AddComponent() {
    components_factory_->Add<T>();
    return *this;
  }

 private:
  HttpHandlersFactory http_handlers_factory_;
  HttpFiltersFactory http_filters_factory_;
  config::ConfigConstPtr config_{std::make_shared<config::Config>()};
  components::ComponentsFactoryPtr components_factory_{
      std::make_shared<components::ComponentsFactory>()};
};

using RouterAndHttpFiltersPair =
    std::pair<router::RouterPtr, HttpFilterContextsMapImpl>;

RouterAndHttpFiltersPair MakeRouterAndHttpFilterCtxsMap(
    const HttpHandlersFactory& http_handlers_factory,
    const HttpFiltersFactory& http_filters_factory,
    const config::Config& config, components::Components& components,
    const ExceptionsHandlerConstPtr& exceptions_handler);
components::ComponentsPtr MakeComponents(
    const YamlConfig& config, const components::ComponentsFactoryPtr& factory);

}  // namespace engine::http_server