#pragma once

#include "caches/currencies_cache.hpp"
#include "caches/exchange_rates_cache.hpp"
#include "engine/src/http_server/http_handler.hpp"
#include "engine/src/http_server/http_request.hpp"
#include "engine/src/http_server/http_response.hpp"

namespace currexch_service::handlers::exchange_rate::get {

class Handler : public engine::http_server::HttpHandlerJson {
 public:
  static constexpr std::string_view kName{"get_exchange_rate"};

  Handler(const engine::YamlConfig& config,
          engine::components::Components& components);

  void HandleRequestJson(
      const engine::http_server::HttpRequestConstPtr& request,
      const engine::http_server::HttpResponsePtr& response,
      const engine::Json& body_json) const override;

 private:
  const caches::CurrenciesCachePtr curr_cache_;
  const caches::ExchangeRatesCachePtr exch_rates_cache_;
};

}  // namespace currexch_service::handlers::exchange_rate::get