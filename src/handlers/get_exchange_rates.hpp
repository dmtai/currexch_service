#pragma once

#include "caches/exchange_rates_cache.hpp"
#include "engine/src/http_server/http_handler.hpp"
#include "engine/src/http_server/http_request.hpp"
#include "engine/src/http_server/http_response.hpp"
#include "models/exchange_rate.hpp"

namespace currexch_service::handlers::exchange_rates::get {

class Handler : public engine::http_server::HttpHandlerJson {
 public:
  static constexpr std::string_view kName{"get_exchange_rates"};

  Handler(const engine::YamlConfig& config,
          engine::components::Components& components);

  void HandleRequestJson(
      const engine::http_server::HttpRequestConstPtr& request,
      const engine::http_server::HttpResponsePtr& response,
      const engine::Json& body_json) const override;

 private:
  const caches::ExchangeRatesCachePtr exch_rates_cache_;
};

}  // namespace currexch_service::handlers::exchange_rates::get