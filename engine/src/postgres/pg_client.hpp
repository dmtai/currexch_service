#pragma once

#include <fmt/core.h>
#include <ozo/connection_info.h>
#include <ozo/connection_pool.h>
#include <ozo/execute.h>
#include <ozo/request.h>
#include <ozo/shortcuts.h>
#include <ozo/transaction.h>
#include <chrono>
#include <memory>
#include "components/components.hpp"
#include "components/io_context.hpp"
#include "utils/config.hpp"
#include "utils/net.hpp"

namespace engine::postgres {

using PgErrorCode = ozo::error_code;
template <typename... Args>
using Rows = ozo::rows_of<Args...>;

class PgClient : public engine::components::ComponentBase {
 public:
  using OzoConnectionPool =
      ozo::connection_pool<ozo::connection_info<ozo::oid_map_t<>, ozo::none_t>,
                           ozo::thread_safety<true>>;

  static constexpr std::string_view kName{"pg_client"};

  PgClient(const YamlConfig& config,
           engine::components::Components& components);

  void Execute(const auto& query, const auto completion_token) {
    ozo::execute(connection_pool_[io_context_->GetRawIOContextRef()], query,
                 std::chrono::milliseconds(5000),
                 [completion_token = std::move(completion_token)](
                     PgErrorCode ec, auto conn) {
                   completion_token(std::move(ec), std::move(conn));
                 });
  }

  template <typename T>
  void Execute(const auto& query, const auto completion_token,
               net::io_context& io_context) const {
    const auto rows = std::make_shared<T>();
    ozo::request(connection_pool_[io_context], query,
                 std::chrono::milliseconds(5000), ozo::into(*rows),
                 [rows, completion_token = std::move(completion_token)](
                     PgErrorCode ec, auto conn) {
                   completion_token(std::move(ec), std::move(conn),
                                    std::move(rows));
                 });
  }

  template <typename T>
  void Execute(const auto& query, const auto completion_token) const {
    Execute<T>(query, std::move(completion_token),
               io_context_->GetRawIOContextRef());
  }

 private:
  mutable engine::components::IOContextBasePtr io_context_;
  mutable OzoConnectionPool connection_pool_;
};

using PgClientPtr = std::shared_ptr<PgClient>;

}  // namespace engine::postgres