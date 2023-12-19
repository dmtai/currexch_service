#include "server_context.hpp"
#include "server.hpp"

namespace engine::http_server {

ServerContextImpl::ServerContextImpl(const Server& server) : server_{server} {}

const router::Router& ServerContextImpl::GetRouter() const noexcept {
  return server_.GetRouter();
}

const config::Config& ServerContextImpl::GetConfig() const noexcept {
  return server_.GetConfig();
}

const HttpFilterContextsMap& ServerContextImpl::GetHttpFilterContextsMap()
    const noexcept {
  return server_.GetHttpFilterContextsMap();
}

const ExceptionsHandler& ServerContextImpl::GetExceptionsHandler()
    const noexcept {
  return server_.GetExceptionsHandler();
}

}  // namespace engine::http_server