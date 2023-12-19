#include "server.hpp"
#include "listener.hpp"
#include "postgres/pg_cache.hpp"
#include "request_handler.hpp"
#include "utils/logger.hpp"

namespace engine::http_server {

ServerImpl::ServerImpl(config::ConfigConstPtr config, router::RouterPtr router,
                       HttpFilterContextsMapImpl&& http_filter_contexts_map,
                       ExceptionsHandlerConstPtr exceptions_handler,
                       components::ComponentsPtr components)
    : config_{std::move(config)},
      router_{std::move(router)},
      http_filter_contexts_map_{std::move(http_filter_contexts_map)},
      exceptions_handler_{std::move(exceptions_handler)},
      components_{std::move(components)},
      io_context_{components_->Get<engine::components::ServerIOContext>()},
      signal_set_{io_context_->GetRawIOContextRef(), SIGINT, SIGTERM},
      server_context_{std::make_shared<ServerContextImpl>(*this)} {
  SetSigtermAndSigintHandler();
}

void ServerImpl::Run() {
  const auto address = net::ip::make_address(config_->general_.address_);
  const net::ip::port_type port = config_->general_.port_;
  MakeListener<RequestHandler>(
      io_context_->GetRawIOContextRef(), {address, port},
      RequestHandler{*server_context_}, *server_context_)
      ->Run();
  LOG(info, "Server started on port={} address={}", config_->general_.port_,
      config_->general_.address_);
  components_->ForEachComponentOfType<components::IOContextBase>(
      [](auto& io_context) { io_context.Run(); });
  components_->ForEachComponentOfType<components::IOContextBase>(
      [](auto& io_context) { io_context.Join(); });
}

void ServerImpl::Stop() {
  components_->ForEachComponentOfType<components::IOContextBase>(
      [](auto& io_context) { io_context.Stop(); });
  LOG(info, "Server exited");
}

void ServerImpl::SetSigtermAndSigintHandler() {
  const auto sig_handler = [this](const boost::system::error_code& ec,
                                  int signal_number) {
    if (ec) {
      LOG(error, "Signal handler error. signal_number={}, code={}, msg={}",
          signal_number, ec.value(), ec.message());
      return;
    }
    Stop();
  };
  signal_set_.async_wait(sig_handler);
}

const router::Router& ServerImpl::GetRouter() const noexcept {
  return *router_;
}

const config::Config& ServerImpl::GetConfig() const noexcept {
  return *config_;
}

const HttpFilterContextsMap& ServerImpl::GetHttpFilterContextsMap()
    const noexcept {
  return http_filter_contexts_map_;
}

const ExceptionsHandler& ServerImpl::GetExceptionsHandler() const noexcept {
  return *exceptions_handler_;
}

}  // namespace engine::http_server