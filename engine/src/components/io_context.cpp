#include "io_context.hpp"
#include <boost/exception/diagnostic_information.hpp>
#include "utils/logger.hpp"

namespace engine::components {

namespace config {

struct IOContextConfig {
  size_t threads_num_{1};
};

IOContextConfig ParseIOContextConfig(const YamlConfig& yaml_config) {
  IOContextConfig config{};
  if (yaml_config["threads_num"].IsDefined()) {
    config.threads_num_ = yaml_config["threads_num"].as<size_t>();
  }
  return config;
}

}  // namespace config

IOContextBase::IOContextBase(const YamlConfig& config)
    : io_context_{std::make_shared<net::io_context>()},
      thread_pool_{config::ParseIOContextConfig(config).threads_num_} {}

IOContextBase::Strand IOContextBase::MakeStrand() const {
  return net::make_strand(*io_context_);
}

net::io_context& IOContextBase::GetRawIOContextRef() const noexcept {
  return *io_context_;
}

void IOContextBase::Run() {
  const auto thread_cb = [this] {
    while (!io_context_->stopped()) {
      try {
        io_context_->run();
      } catch (const std::exception& ex) {
        LOG(error, "Unhandled exception: {}", ex.what());
      } catch (...) {
        LOG(error, "Unknown exception: {}",
            boost::current_exception_diagnostic_information());
      }
    }
  };
  thread_pool_.Run(thread_cb);
}

void IOContextBase::Stop() { io_context_->stop(); }

void IOContextBase::Join() { thread_pool_.JoinAll(); }

size_t IOContextBase::GetThreadsNum() const noexcept {
  return thread_pool_.GetThreadsNum();
}

ServerIOContext::ServerIOContext(
    const engine::YamlConfig& config,
    [[maybe_unused]] engine::components::Components& components)
    : IOContextBase(config) {}

PgCachesIOContext::PgCachesIOContext(
    const engine::YamlConfig& config,
    [[maybe_unused]] engine::components::Components& components)
    : IOContextBase(config) {}

}  // namespace engine::components