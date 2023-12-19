#pragma once

#include <boost/functional/hash.hpp>
#include <libcuckoo/cuckoohash_map.hh>
#include <memory>
#include <unordered_map>
#include "engine/src/utils/utils.hpp"
#include "currencies_cache.hpp"
#include "engine/src/postgres/pg_cache.hpp"
#include "models/exchange_rate.hpp"

namespace currexch_service::caches {

class ExchangeRatesCache
    : public engine::postgres::PgCache<ExchangeRatesCache,
                                       models::ExchangeRate> {
 public:
  struct ExchangeRate {
    models::ExchangeRate model_;
  };

  using ExchangeRateConstPtr = std::shared_ptr<const ExchangeRate>;
  using AllExchangeRatesJsonConstPtr = std::shared_ptr<const std::string>;
  using AllExchangeRateJsonAtomicConstPtr =
      std::atomic<AllExchangeRatesJsonConstPtr>;
  using ExchangeRateById =
      libcuckoo::cuckoohash_map<std::int32_t, ExchangeRateConstPtr>;
  using CurrencyPair = std::pair<std::int32_t, std::int32_t>;
  using ExchangeRateByCurrPair =
      libcuckoo::cuckoohash_map<CurrencyPair, ExchangeRateConstPtr,
                                boost::hash<CurrencyPair>>;
  using ExchangeRateByIdStdMap =
      std::unordered_map<std::int32_t, ExchangeRateConstPtr>;

  static constexpr std::string_view kName{"exchange_rates_cache"};
  static constexpr std::string_view kUpdateQuery{
      sql::kUpdateExchangeRatesCache};
  static constexpr std::string_view kFullUpdateQuery{
      sql::kFullUpdateExchangeRatesCache};

  ExchangeRatesCache(const engine::YamlConfig& config,
                     engine::components::Components& components);

  void FullUpdateCache(const Rows& rows) override;
  void UpdateCache(const Rows& rows) override;
  AllExchangeRatesJsonConstPtr GetAllExchangeRatesJson() const noexcept;
  ExchangeRateConstPtr GetExchangeRateByCurrPair(
      std::int32_t base_curr_id, std::int32_t target_curr_id) const;

 private:
  AllExchangeRatesJsonConstPtr AllExchangeRatesToJson(
      bool is_exch_rates_updated);

  const engine::postgres::PgClientPtr pg_client_;
  const CurrenciesCachePtr curr_cache_;
  ExchangeRateById exchange_rate_by_id_;
  ExchangeRateByCurrPair exchange_rate_by_curr_pair_;
  AllExchangeRateJsonAtomicConstPtr all_exchange_rates_;
  ExchangeRateByIdStdMap exchange_rate_by_id_std_map_;
  bool has_skipped_exch_rates_;
};

using ExchangeRatesCachePtr = std::shared_ptr<ExchangeRatesCache>;

}  // namespace currexch_service::caches