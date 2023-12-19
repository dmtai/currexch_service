#include "get_exchange_rates.hpp"

namespace currexch_service::handlers::exchange_rates::get {

Handler::Handler([[maybe_unused]] const engine::YamlConfig& config,
                 engine::components::Components& components)
    : exch_rates_cache_{components.Get<caches::ExchangeRatesCache>()} {}

void Handler::HandleRequestJson(
    [[maybe_unused]] const engine::http_server::HttpRequestConstPtr& request,
    const engine::http_server::HttpResponsePtr& response,
    [[maybe_unused]] const engine::Json& body_json) const {
  return response->Send(*exch_rates_cache_->GetAllExchangeRatesJson());
}

}  // namespace currexch_service::handlers::exchange_rates::get