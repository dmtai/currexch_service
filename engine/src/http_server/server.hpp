#pragma once

#include <boost/asio/signal_set.hpp>
#include <memory>
#include <string>
#include "components/components.hpp"
#include "components/io_context.hpp"
#include "config.hpp"
#include "exceptions_handler.hpp"
#include "http_filter_ctxs_map.hpp"
#include "router.hpp"
#include "server_context.hpp"
#include "utils/net.hpp"
#include "utils/thread_pool.hpp"

namespace engine::http_server {

class Server {
 public:
  virtual ~Server() = default;

  virtual void Run() = 0;
  virtual void Stop() = 0;

  virtual const router::Router& GetRouter() const noexcept = 0;
  virtual const config::Config& GetConfig() const noexcept = 0;
  virtual const HttpFilterContextsMap& GetHttpFilterContextsMap()
      const noexcept = 0;
  virtual const ExceptionsHandler& GetExceptionsHandler() const noexcept = 0;
};

class ServerImpl : public Server {
 public:
  ServerImpl() = delete;
  ServerImpl(const ServerImpl&) = delete;
  ServerImpl& operator=(const ServerImpl&) = delete;
  ServerImpl(ServerImpl&&) = default;
  ServerImpl& operator=(ServerImpl&&) = default;
  ServerImpl(config::ConfigConstPtr config, router::RouterPtr router,
             HttpFilterContextsMapImpl&& http_filter_contexts_map,
             ExceptionsHandlerConstPtr exceptions_handler,
             components::ComponentsPtr components);

  void Run() override;
  void Stop() override;

  const router::Router& GetRouter() const noexcept override;
  const config::Config& GetConfig() const noexcept override;
  const HttpFilterContextsMap& GetHttpFilterContextsMap()
      const noexcept override;
  const ExceptionsHandler& GetExceptionsHandler() const noexcept override;

 private:
  void SetSigtermAndSigintHandler();

  config::ConfigConstPtr config_;
  router::RouterPtr router_;
  HttpFilterContextsMapImpl http_filter_contexts_map_;
  ExceptionsHandlerConstPtr exceptions_handler_;
  components::ComponentsPtr components_;
  components::IOContextBasePtr io_context_;
  net::signal_set signal_set_;
  ServerContextConstPtr server_context_;
};

using ServerPtr = std::unique_ptr<Server>;

}  // namespace engine::http_server