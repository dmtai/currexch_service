#pragma once

#include <memory>
#include "component_base.hpp"
#include "components.hpp"
#include "utils/config.hpp"
#include "utils/net.hpp"
#include "utils/thread_pool.hpp"

namespace engine::components {

using IOContextPtr = std::shared_ptr<net::io_context>;

class IOContextBase : public ComponentBase {
 public:
  using Strand = boost::asio::strand<boost::asio::io_context::executor_type>;

  explicit IOContextBase(const YamlConfig& config);
  void Run();
  void Stop();
  void Join();
  size_t GetThreadsNum() const noexcept;
  [[nodiscard]] Strand MakeStrand() const;
  [[nodiscard]] net::io_context& GetRawIOContextRef() const noexcept;

 private:
  const IOContextPtr io_context_;
  utils::ThreadPool thread_pool_;
};

using IOContextBasePtr = std::shared_ptr<IOContextBase>;

class ServerIOContext : public IOContextBase {
 public:
  static constexpr std::string_view kName{"server_io_context"};

  ServerIOContext(const engine::YamlConfig& config,
                  engine::components::Components& components);
};

using ServerIOContextPtr = std::shared_ptr<ServerIOContext>;

class PgCachesIOContext : public IOContextBase {
 public:
  static constexpr std::string_view kName{"pg_caches_io_context"};

  PgCachesIOContext(const engine::YamlConfig& config,
                    engine::components::Components& components);
};

using PgCachesIOContextPtr = std::shared_ptr<PgCachesIOContext>;

}  // namespace engine::components