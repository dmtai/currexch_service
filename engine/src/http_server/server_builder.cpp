#include "server_builder.hpp"
#include <fmt/core.h>
#include <array>
#include <memory>
#include <optional>
#include <vector>
#include "components/components.hpp"
#include "components/io_context.hpp"
#include "config.hpp"
#include "postgres/pg_cache.hpp"
#include "utils/thread_pool.hpp"

namespace engine::http_server {

namespace {

void FillFilterCtxsMap(HttpFilterContextsMapImpl& http_filters_ctxs_map,
                       config::HttpHandler::HttpFiltersVec filters_config,
                       const std::string& handler_name,
                       const HttpHandlerBaseConstPtr& handler,
                       const HttpFiltersFactory& http_filters_factory,
                       components::Components& components,
                       const ExceptionsHandlerConstPtr& exceptions_handler) {
  if (filters_config.empty()) {
    return;
  }
  HttpFilterContextPtr prev_filter_ctx{nullptr};
  for (auto it = filters_config.rbegin(); it != filters_config.rend(); ++it) {
    const auto& [filter_name, filter_config] = *it;
    const auto filter =
        http_filters_factory.Create(filter_name, filter_config, components);
    if (!filter) {
      throw std::runtime_error{
          fmt::format("HTTP filter \"{}\" isn't registred", filter_name)};
    }
    const auto cur_filter_ctx = std::make_shared<HttpFilterContextImpl>(
        filter, prev_filter_ctx, handler, exceptions_handler);
    prev_filter_ctx = cur_filter_ctx;
    http_filters_ctxs_map.AddFront(handler_name, prev_filter_ctx);
  }
}

void InitPgCaches(components::Components& components) {
  net::io_context io_context;

  size_t caches_num{};
  const auto async_run_caches_init = [&](auto& cache) {
    cache.AsyncRunInit(io_context);
    ++caches_num;
  };
  components.ForEachComponentOfType<engine::postgres::PgCacheBase>(
      async_run_caches_init);
  if (!caches_num) {
    return;
  }
  utils::ThreadPool thread_pool{caches_num};
  LOG(debug, "Start caches initialization");
  thread_pool.Run([&] { io_context.run(); });
  thread_pool.JoinAll();
  LOG(debug, "Caches initialization completed");
}

void RunPgCachesUpdate(components::Components& components) {
  components.ForEachComponentOfType<engine::postgres::PgCacheBase>(
      [](auto& cache) { cache.AsyncRunUpdate(); });
}

}  // namespace

RouterAndHttpFiltersPair MakeRouterAndHttpFilterCtxsMap(
    const HttpHandlersFactory& http_handlers_factory,
    const HttpFiltersFactory& http_filters_factory,
    const config::Config& config, components::Components& components,
    const ExceptionsHandlerConstPtr& exceptions_handler) {
  auto router = std::make_shared<router::Router>();
  HttpFilterContextsMapImpl http_filters_ctxs_map;
  for (const auto& [handler_name, handler_config] : config.http_handlers_) {
    const auto handler = http_handlers_factory.Create(
        handler_name, handler_config.yaml_config_, components);
    if (!handler) {
      throw std::runtime_error{
          fmt::format("HTTP handler \"{}\" isn't registred", handler_name)};
    }
    const auto http_method = ToHttpMethodEnum(handler_config.method_);
    if (http_method == HttpMethod::kUnknown) {
      throw std::runtime_error{
          fmt::format("Unknown HTTP method \"{}\" in HTTP handler \"{}\"",
                      handler_config.method_, handler_name)};
    }
    router->Add(http_method, handler_config.path_, handler_name, handler);
    FillFilterCtxsMap(http_filters_ctxs_map, handler_config.filters_,
                      handler_name, handler, http_filters_factory, components,
                      exceptions_handler);
  }
  return {router, std::move(http_filters_ctxs_map)};
}

components::ComponentsPtr MakeComponents(
    const YamlConfig& config, const components::ComponentsFactoryPtr& factory) {
  const auto components =
      std::make_shared<components::Components>(config, factory);
  const auto make_components = [&](const auto& name,
                                   const auto& objCreationCb) {
    if (components->Contains(name)) {
      return;
    }
    const auto new_component = objCreationCb(
        engine::config::RetrieveValidOrEmptyConfig(name, config), *components);
    components->Add(name, new_component);
  };
  factory->ForEach(make_components);

  InitPgCaches(*components);
  RunPgCachesUpdate(*components);

  return components;
}

ServerBuilder& ServerBuilder::SetConfig(config::ConfigConstPtr config) {
  config_ = std::move(config);
  return *this;
}

ServerPtr ServerBuilder::Build() {
  const auto components =
      MakeComponents(config_->components_, components_factory_);
  const auto exceptions_handler =
      std::make_shared<const ExceptionsHandlerImpl>();
  auto [router, http_filter_ctxs_map] = MakeRouterAndHttpFilterCtxsMap(
      http_handlers_factory_, http_filters_factory_, *config_, *components,
      exceptions_handler);
  return std::make_unique<ServerImpl>(
      std::make_shared<const config::Config>(*config_), std::move(router),
      std::move(http_filter_ctxs_map), std::move(exceptions_handler),
      std::move(components));
}

}  // namespace engine::http_server