#include "get_exchange_rate.hpp"
#include "dto/dto.hpp"
#include "engine/src/http_server/http_status.hpp"
#include "models/currency.hpp"
#include "models/exchange_rate.hpp"
#include "utils/make_error.hpp"
#include "utils/utils.hpp"

namespace currexch_service::handlers::exchange_rate::get {

Handler::Handler([[maybe_unused]] const engine::YamlConfig& config,
                 engine::components::Components& components)
    : curr_cache_{components.Get<caches::CurrenciesCache>()},
      exch_rates_cache_{components.Get<caches::ExchangeRatesCache>()} {}

void Handler::HandleRequestJson(
    [[maybe_unused]] const engine::http_server::HttpRequestConstPtr& request,
    const engine::http_server::HttpResponsePtr& response,
    [[maybe_unused]] const engine::Json& body_json) const {
  const auto curr_pair_str =
      boost::lexical_cast<std::string>(request->GetMatchResult()["curr_pair"]);
  if (curr_pair_str.empty()) {
    response->SetStatusCode(engine::http_server::HttpStatus::kBadRequest);
    return response->Send(
        utils::MakeError("There is no currency pair in the URL"));
  }

  const auto curr_pair = utils::ParseCurrPair(curr_pair_str);
  if (!curr_pair) {
    response->SetStatusCode(engine::http_server::HttpStatus::kBadRequest);
    return response->Send(utils::MakeError("Invalid currency pair"));
  }

  const auto base_curr = curr_cache_->GetCurrencyByCode(curr_pair->first);
  if (!base_curr) {
    response->SetStatusCode(engine::http_server::HttpStatus::kNotFound);
    return response->Send(utils::MakeError("Exchange rate not found"));
  }
  const auto target_curr = curr_cache_->GetCurrencyByCode(curr_pair->second);
  if (!target_curr) {
    response->SetStatusCode(engine::http_server::HttpStatus::kNotFound);
    return response->Send(utils::MakeError("Exchange rate not found"));
  }

  const auto exch_rate = exch_rates_cache_->GetExchangeRateByCurrPair(
      base_curr->model_.currency_id, target_curr->model_.currency_id);
  if (!exch_rate) {
    response->SetStatusCode(engine::http_server::HttpStatus::kNotFound);
    return response->Send(utils::MakeError("Exchange rate not found"));
  }

  response->Send(dto::MakeExchangeRateDtoJson(exch_rate->model_,
                                              base_curr->dto_json_,
                                              target_curr->dto_json_)
                     .dump());
}

}  // namespace currexch_service::handlers::exchange_rate::get