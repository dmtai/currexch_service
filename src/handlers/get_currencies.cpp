#include "get_currencies.hpp"

namespace currexch_service::handlers::currencies::get {

Handler::Handler([[maybe_unused]] const engine::YamlConfig& config,
                 engine::components::Components& components)
    : curr_cache_{components.Get<caches::CurrenciesCache>()} {}

void Handler::HandleRequestJson(
    [[maybe_unused]] const engine::http_server::HttpRequestConstPtr& request,
    const engine::http_server::HttpResponsePtr& response,
    [[maybe_unused]] const engine::Json& body_json) const {
  response->Send(*curr_cache_->GetAllCurrenciesJson());
}

}  // namespace currexch_service::handlers::currencies::get