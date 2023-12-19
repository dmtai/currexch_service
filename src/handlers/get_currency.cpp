#include "get_currency.hpp"
#include "engine/src/http_server/http_status.hpp"
#include "utils/make_error.hpp"

namespace currexch_service::handlers::currency::get {

Handler::Handler([[maybe_unused]] const engine::YamlConfig& config,
                 engine::components::Components& components)
    : curr_cache_{components.Get<caches::CurrenciesCache>()} {}

void Handler::HandleRequestJson(
    [[maybe_unused]] const engine::http_server::HttpRequestConstPtr& request,
    const engine::http_server::HttpResponsePtr& response,
    [[maybe_unused]] const engine::Json& body_json) const {
  const auto code =
      boost::lexical_cast<std::string>(request->GetMatchResult()["code"]);
  if (code.empty()) {
    response->SetStatusCode(engine::http_server::HttpStatus::kBadRequest);
    return response->Send(
        utils::MakeError("There is no currency code in the URL"));
  }
  const auto curr = curr_cache_->GetCurrencyByCode(code);
  if (!curr) {
    response->SetStatusCode(engine::http_server::HttpStatus::kNotFound);
    return response->Send(utils::MakeError("Currency not found"));
  }
  response->Send(curr->dto_json_.dump());
}

}  // namespace currexch_service::handlers::currency::get