#pragma once

#include <fmt/core.h>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include "components/component_base.hpp"
#include "components/components.hpp"
#include "components/io_context.hpp"
#include "pg_client.hpp"
#include "utils/config.hpp"
#include "utils/logger.hpp"
#include "utils/net.hpp"
#include "utils/thread_pool.hpp"
#include "utils/utils.hpp"

namespace engine::postgres {

class PgCacheBase : public engine::components::ComponentBase {
 public:
  virtual void AsyncRunInit(net::io_context& io_context) = 0;
  virtual void AsyncRunUpdate() = 0;
};

using PgCacheBasePtr = std::shared_ptr<PgCacheBase>;

namespace config {

struct PgCacheConfig {
  std::chrono::milliseconds update_timeout_{2000};
  std::chrono::milliseconds full_update_timeout_{20000};
};

PgCacheConfig ParsePgCacheConfig(const YamlConfig& yaml_config);

}  // namespace config

template <typename RowType>
class PgCacheUpdaterBase {
 public:
  using Row = RowType;
  using Rows = std::vector<Row>;
  using RowsPtr = std::shared_ptr<Rows>;
  using UpdateFunc = std::function<void(Rows)>;

  PgCacheUpdaterBase(net::io_context& io_context,
                     net::any_io_executor io_executor,
                     std::chrono::milliseconds timeout,
                     std::string_view cache_name)
      : update_timer_{io_context},
        io_executor_{io_executor},
        update_timeout_{std::move(timeout)},
        cache_name_{cache_name} {}

  virtual void DoQuery() = 0;
  virtual void UpdateCache(const Rows& rows) = 0;

  void AsyncRunUpdate() { SetUpdateTimer(); }

  void SetUpdateTimer() {
    update_timer_.expires_after(update_timeout_);
    update_timer_.async_wait(
        std::bind_front(&PgCacheUpdaterBase::OnUpdateTimer, this));
  }

  void OnUpdateTimer(boost::system::error_code ec) {
    if (ec) {
      LOG(critical, "Error of \"{}\" cache update. {}", cache_name_,
          ec.message());
      SetUpdateTimer();
    }
    DoQuery();
  }

  void OnQueryCompletion(auto ec, auto conn, auto rows) {
    if (ec) {
      LOG(critical, "Error of \"{}\" cache update. Msg={}. libpq msg={}",
          cache_name_, ec.message(), ozo::error_message(conn));
      return SetUpdateTimer();
    }
    net::post(io_executor_,
              [this, rows = std::move(rows)]() { UpdateCache(*rows); });
  }

 protected:
  net::steady_timer update_timer_;
  net::any_io_executor io_executor_;
  std::chrono::milliseconds update_timeout_;
  std::string_view cache_name_;
};

template <typename RowType>
class PgCacheUpdater : public PgCacheUpdaterBase<RowType> {
 public:
  PgCacheUpdater(net::io_context& io_context, net::any_io_executor io_executor,
                 std::chrono::milliseconds timeout, PgClientPtr pg_client,
                 typename PgCacheUpdaterBase<RowType>::UpdateFunc update_func,
                 std::string_view query, std::string_view cache_name)
      : PgCacheUpdaterBase<RowType>{io_context, std::move(io_executor),
                                    std::move(timeout), cache_name},
        io_context_{io_context},
        pg_client_{std::move(pg_client)},
        update_func_{std::move(update_func)},
        query_{query} {}

  void DoQuery() override {
    update_time_ = utils::GetSystemTimeWithTimezone();
    pg_client_->Execute<typename PgCacheUpdaterBase<RowType>::Rows>(
        GetQuery(),
        [this](auto ec, auto conn, auto rows) {
          PgCacheUpdaterBase<RowType>::OnQueryCompletion(
              std::move(ec), std::move(conn), std::move(rows));
        },
        io_context_);
  }

  void UpdateCache(
      const typename PgCacheUpdaterBase<RowType>::Rows& rows) override {
    last_update_time_ = update_time_;
    update_func_(rows);
    PgCacheUpdaterBase<RowType>::SetUpdateTimer();
  }

  const std::string& GetLastUpdateTime() { return last_update_time_; }

  auto GetQuery() { return ozo::make_query(query_, GetLastUpdateTime()); }

  void SetLastUpdateTime(const std::string& update_time) {
    last_update_time_ = update_time_;
  }

 private:
  net::io_context& io_context_;
  PgClientPtr pg_client_;
  std::string update_time_;
  std::string last_update_time_;
  PgCacheUpdaterBase<RowType>::UpdateFunc update_func_;
  std::string_view query_;
};

template <typename RowType>
class PgCacheFullUpdater : public PgCacheUpdaterBase<RowType> {
 public:
  PgCacheFullUpdater(net::io_context& io_context,
                     net::any_io_executor io_executor,
                     std::chrono::milliseconds timeout, PgClientPtr pg_client,
                     PgCacheUpdaterBase<RowType>::UpdateFunc update_func,
                     std::string_view query, std::string_view cache_name)
      : PgCacheUpdaterBase<RowType>{io_context, std::move(io_executor),
                                    std::move(timeout), cache_name},
        io_context_{io_context},
        pg_client_{std::move(pg_client)},
        update_func_{std::move(update_func)},
        query_{query} {}

  void DoQuery() override {
    pg_client_->Execute<Rows>(
        GetQuery(),
        [this](auto ec, auto conn, auto rows) {
          OnQueryCompletion(std::move(ec), std::move(conn), std::move(rows));
        },
        io_context_);
  }

  void UpdateCache(const PgCacheUpdaterBase<RowType>::Rows& rows) override {
    update_func_(*rows);
    PgCacheUpdaterBase<RowType>::SetUpdateTimer();
  }

  auto GetQuery() { return ozo::make_query(query_); }

 private:
  net::io_context& io_context_;
  PgClientPtr pg_client_;
  typename PgCacheUpdaterBase<RowType>::UpdateFunc update_func_;
  std::string_view query_;
};

template <typename Derived, typename RowType>
class PgCache : public PgCacheBase {
 public:
  using Row = RowType;
  using Rows = std::vector<Row>;

  PgCache(const YamlConfig& config,
          engine::components::IOContextBasePtr io_context,
          net::any_io_executor io_executor,
          engine::postgres::PgClientPtr pg_client)
      : config_{config::ParsePgCacheConfig(config)},
        updater_{io_context->GetRawIOContextRef(),
                 io_executor,
                 config_.update_timeout_,
                 pg_client,
                 std::bind_front(&PgCache::UpdateCache, this),
                 Derived::kUpdateQuery,
                 Derived::kName},
        full_updater_{io_context->GetRawIOContextRef(),
                      io_executor,
                      config_.full_update_timeout_,
                      pg_client,
                      std::bind_front(&PgCache::FullUpdateCache, this),
                      Derived::kFullUpdateQuery,
                      Derived::kName},
        pg_client_{pg_client},
        is_init_{false} {}

  virtual void FullUpdateCache(const Rows& rows) = 0;
  virtual void UpdateCache(const Rows& row) = 0;

  void AsyncRunUpdate() {
    updater_.SetLastUpdateTime(last_update_time_);
    updater_.AsyncRunUpdate();
    full_updater_.AsyncRunUpdate();
  }

  void AsyncRunInit(net::io_context& io_context) override {
    DoInitQuery(io_context);
  }

  void DoInitQuery(net::io_context& io_context) {
    last_update_time_ = utils::GetSystemTimeWithTimezone();
    pg_client_->Execute<Rows>(
        ozo::make_query(Derived::kFullUpdateQuery),
        [this](auto ec, auto conn, auto rows) {
          OnInitQueryCompletion(std::move(ec), std::move(conn),
                                std::move(rows));
        },
        io_context);
  }

  void OnInitQueryCompletion(auto ec, auto conn, auto rows) {
    if (ec) {
      throw std::runtime_error(fmt::format(
          "Error of \"{}\" cache initialization. Msg={}. libpq msg={}",
          Derived::kName, ec.message(), ozo::error_message(conn)));
    }
    UpdateCache(*rows);
    NotifyAboutInitCompletion();
  }

  void WaitForInit() {
    if (is_init_.load()) {
      return;
    }
    std::unique_lock<std::mutex> lk{mtx_};
    cv_.wait(lk, [this] { return is_init_.load(); });
  }

  void NotifyAboutInitCompletion() {
    std::unique_lock<std::mutex> lk{mtx_};
    is_init_ = true;
    lk.unlock();
    cv_.notify_all();
  }

 private:
  const config::PgCacheConfig config_;
  PgCacheUpdater<RowType> updater_;
  PgCacheUpdater<RowType> full_updater_;
  const PgClientPtr pg_client_;
  std::string last_update_time_;
  std::atomic_bool is_init_;
  std::mutex mtx_;
  std::condition_variable cv_;
};

}  // namespace engine::postgres
