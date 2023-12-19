#pragma once

#include <memory>
#include <string>
#include "config.hpp"
#include "router.hpp"

namespace engine::http_server {

class Server;
class HttpFilterContextsMap;
using ExceptionsHandlerConstPtr =
    std::shared_ptr<const class ExceptionsHandler>;

class ServerContext {
 public:
  virtual ~ServerContext() = default;

  virtual const router::Router& GetRouter() const noexcept = 0;
  virtual const config::Config& GetConfig() const noexcept = 0;
  virtual const HttpFilterContextsMap& GetHttpFilterContextsMap()
      const noexcept = 0;
  virtual const ExceptionsHandler& GetExceptionsHandler() const noexcept = 0;
};

class ServerContextImpl : public ServerContext {
 public:
  explicit ServerContextImpl(const Server& server);

  const router::Router& GetRouter() const noexcept override;
  const config::Config& GetConfig() const noexcept override;
  const HttpFilterContextsMap& GetHttpFilterContextsMap()
      const noexcept override;
  const ExceptionsHandler& GetExceptionsHandler() const noexcept override;

 private:
  const Server& server_;
};

using ServerContextConstPtr = std::shared_ptr<const ServerContext>;

}  // namespace engine::http_server