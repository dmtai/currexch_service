#include "post_exchange_rates.hpp"
#include <boost/algorithm/string.hpp>
#include "dto/dto.hpp"
#include "engine/src/http_server/exceptions.hpp"
#include "engine/src/http_server/http_status.hpp"
#include "engine/src/utils/x_www_form_urlencoded.hpp"
#include "models/currency.hpp"
#include "models/exchange_rate_with_currencies.hpp"
#include "sql/sql.hpp"
#include "utils/make_error.hpp"

namespace currexch_service::handlers::exchange_rates::post {

namespace {

struct RequestData {
  std::string base_currency_code;
  std::string target_currency_code;
  std::string rate;
};

using ExchangeRateIDs = std::vector<std::int32_t>;

RequestData ParseRequest(const engine::http_server::HttpRequest& request) {
  auto data = engine::utils::form_urlencoded::ParseXWWWWFormUrlencodedStr(
      request.GetBody());

  const auto base_currency_code_it = data.find("baseCurrencyCode");
  if (base_currency_code_it == data.end() ||
      base_currency_code_it->second.empty()) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'baseCurrencyCode' is required"),
        engine::http_server::HttpStatus::kBadRequest};
  }
  auto& base_currency_code = base_currency_code_it->second;
  if (base_currency_code.size() > 3) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'baseCurrencyCode' is invalid"),
        engine::http_server::HttpStatus::kBadRequest};
  }

  const auto target_currency_code_it = data.find("targetCurrencyCode");
  if (target_currency_code_it == data.end() ||
      target_currency_code_it->second.empty()) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'targetCurrencyCode' is required"),
        engine::http_server::HttpStatus::kBadRequest};
  }
  auto& target_currency_code = target_currency_code_it->second;
  if (target_currency_code.size() > 3) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'targetCurrencyCode' is invalid"),
        engine::http_server::HttpStatus::kBadRequest};
  }

  const auto rate_it = data.find("rate");
  if (rate_it == data.end() || rate_it->second.empty()) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'rate' is required"),
        engine::http_server::HttpStatus::kBadRequest};
  }
  auto& rate = rate_it->second;
  if (rate.size() > 15) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'rate' is invalid"),
        engine::http_server::HttpStatus::kBadRequest};
  }

  boost::algorithm::to_upper(base_currency_code);
  boost::algorithm::to_upper(target_currency_code);
  return {std::move(base_currency_code), std::move(target_currency_code),
          std::move(rate)};
}

}  // namespace

Handler::Handler([[maybe_unused]] const engine::YamlConfig& config,
                 engine::components::Components& components)
    : pg_client_{components.Get<engine::postgres::PgClient>()},
      curr_cache_{components.Get<caches::CurrenciesCache>()} {}

void Handler::HandleRequest(
    [[maybe_unused]] const engine::http_server::HttpRequestConstPtr& request,
    const engine::http_server::HttpResponsePtr& response) const {
  const auto request_data = ParseRequest(*request);

  const auto insert_handler = [pg_client = pg_client_, response](
                                  const auto& ec, const auto& conn,
                                  const auto& ids) {
    if (ec || ids->size() != 1) {
      if (ec == ozo::sqlstate::code::unique_violation) {
        response->SetStatusCode(engine::http_server::HttpStatus::kConflict);
        return response->Send(utils::MakeError("Exchange rate already exists"));
      }
      response->SetStatusCode(
          engine::http_server::HttpStatus::kInternalServerError);
      return response->Send(utils::MakeError("DB error"));
    }

    const auto select_handler = [pg_client, response](const auto& ec,
                                                      const auto& conn,
                                                      const auto& exch_rates) {
      if (ec || exch_rates->size() != 1) {
        response->SetStatusCode(
            engine::http_server::HttpStatus::kInternalServerError);
        return response->Send(utils::MakeError("DB error"));
      }
      auto& exch_rate_with_curr = (*exch_rates)[0];
      return response->Send(
          dto::MakeExchangeRateWithCurrDtoJson(exch_rate_with_curr).dump());
    };
    auto& exch_rate_id = (*ids)[0];
    pg_client->Execute<models::ExchangeRateWithCurrenciesVec>(
        ozo::make_query(sql::kSelectExchangeRateWithCurrenciesById,
                        exch_rate_id),
        select_handler);
  };

  const auto base_curr =
      curr_cache_->GetCurrencyByCode(request_data.base_currency_code);
  if (!base_curr) {
    response->SetStatusCode(engine::http_server::HttpStatus::kNotFound);
    return response->Send(utils::MakeError("Base currency not found"));
  }
  const auto target_curr =
      curr_cache_->GetCurrencyByCode(request_data.target_currency_code);
  if (!target_curr) {
    response->SetStatusCode(engine::http_server::HttpStatus::kNotFound);
    return response->Send(utils::MakeError("Target currency not found"));
  }

  pg_client_->Execute<ExchangeRateIDs>(
      ozo::make_query(sql::kInsertNewExchangeRate,
                      base_curr->model_.currency_id,
                      target_curr->model_.currency_id, request_data.rate),
      insert_handler);
}

}  // namespace currexch_service::handlers::exchange_rates::post