#pragma once

#include <utility>
#include "caches/currencies_cache.hpp"
#include "caches/exchange_rates_cache.hpp"
#include "engine/src/http_server/http_handler.hpp"
#include "engine/src/http_server/http_request.hpp"
#include "engine/src/http_server/http_response.hpp"
#include "engine/src/postgres/pg_client.hpp"

namespace currexch_service::handlers::exchange::get {

class Handler : public engine::http_server::HttpHandlerBase {
 public:
  using CurrencyPair = std::pair<caches::CurrenciesCache::CurrencyConstPtr,
                                 caches::CurrenciesCache::CurrencyConstPtr>;
  using ExchangeRates =
      std::pair<caches::ExchangeRatesCache::ExchangeRateConstPtr,
                caches::ExchangeRatesCache::ExchangeRateConstPtr>;

  static constexpr std::string_view kName{"get_exchange"};

  Handler(const engine::YamlConfig& config,
          engine::components::Components& components);

  void HandleRequest(
      const engine::http_server::HttpRequestConstPtr& request,
      const engine::http_server::HttpResponsePtr& response) const override;

 private:
  CurrencyPair FindCurrencyPairByCodes(
      const std::string& base_curr_code,
      const std::string& target_curr_code) const;
  caches::ExchangeRatesCache::ExchangeRateConstPtr
  FindExchangeRateByCurrencyPair(const std::int32_t base_curr_id,
                                 const std::int32_t target_curr_id) const;
  ExchangeRates FindUsdExchangeRates(const std::int32_t base_curr_id,
                                     const std::int32_t target_curr_id) const;

  const caches::CurrenciesCachePtr curr_cache_;
  const caches::ExchangeRatesCachePtr exch_rates_cache_;
};

}  // namespace currexch_service::handlers::exchange::get