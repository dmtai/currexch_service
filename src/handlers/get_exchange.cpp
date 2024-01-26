#include "get_exchange.hpp"
#include <boost/algorithm/string.hpp>
#include "utils/multiprecision.hpp"
#include "dto/dto.hpp"
#include "engine/src/http_server/exceptions.hpp"
#include "engine/src/http_server/http_status.hpp"
#include "engine/src/utils/x_www_form_urlencoded.hpp"
#include "models/currency.hpp"
#include "models/exchange_rate_with_currencies.hpp"
#include "sql/sql.hpp"
#include "utils/make_error.hpp"
#include "utils/utils.hpp"
#include "calcs/calcs.hpp"

namespace currexch_service::handlers::exchange::get {

namespace {

struct RequestData {
  std::string base_currency_code_;
  std::string target_currency_code_;
  std::string amount_;
};

RequestData ParseRequest(const engine::http_server::HttpRequest& request) {
  const auto query = request.GetQuery();
  auto base_curr_code = query->find("from");
  if (base_curr_code == query->end() || !base_curr_code->has_value) {
    throw engine::http_server::HttpException{
        utils::MakeError("Parameter 'from' is required"),
        engine::http_server::HttpStatus::kBadRequest};
  }
  if (base_curr_code->value.size() != 3) {
    throw engine::http_server::HttpException{
        utils::MakeError("Parameter 'from' is invalid"),
        engine::http_server::HttpStatus::kBadRequest};
  }

  auto target_curr_code = query->find("to");
  if (target_curr_code == query->end() || !target_curr_code->has_value) {
    throw engine::http_server::HttpException{
        utils::MakeError("Parameter 'to' is required"),
        engine::http_server::HttpStatus::kBadRequest};
  }
  if (target_curr_code->value.size() != 3) {
    throw engine::http_server::HttpException{
        utils::MakeError("Parameter 'to' is invalid"),
        engine::http_server::HttpStatus::kBadRequest};
  }

  auto amount = query->find("amount");
  if (amount == query->end() || !amount->has_value) {
    throw engine::http_server::HttpException{
        utils::MakeError("Parameter 'amount' is required"),
        engine::http_server::HttpStatus::kBadRequest};
  }

  return {std::string{base_curr_code->value},
          std::string{target_curr_code->value}, std::string{amount->value}};
}

}  // namespace

Handler::Handler([[maybe_unused]] const engine::YamlConfig& config,
                 engine::components::Components& components)
    : curr_cache_{components.Get<caches::CurrenciesCache>()},
      exch_rates_cache_{components.Get<caches::ExchangeRatesCache>()} {}

void Handler::HandleRequest(
    [[maybe_unused]] const engine::http_server::HttpRequestConstPtr& request,
    const engine::http_server::HttpResponsePtr& response) const {
  const auto request_data = ParseRequest(*request);

  const auto [base_curr, target_curr] = FindCurrencyPairByCodes(
      request_data.base_currency_code_, request_data.target_currency_code_);

  if (const auto exch_rate = FindExchangeRateByCurrencyPair(
          base_curr->model_.currency_id, target_curr->model_.currency_id)) {
    const Decimal rate{exch_rate->model_.rate};
    const Decimal amount{request_data.amount_};

    const auto converted_amount =
        calcs::CalcConvAmount(rate, amount).str(2, std::ios::fixed);

    return response->Send(dto::MakeConvertedAmountDtoJson(
                              base_curr->model_, target_curr->model_,
                              rate.str(2, std::ios::fixed),
                              amount.str(2, std::ios::fixed), converted_amount)
                              .dump());
  }

  if (const auto exch_rate = FindExchangeRateByCurrencyPair(
          target_curr->model_.currency_id, base_curr->model_.currency_id)) {
    const Decimal rate{exch_rate->model_.rate};
    const Decimal amount{request_data.amount_};
    const auto reverse_rate = calcs::CalcReverseRate(rate);

    const auto converted_amount = calcs::CalcConvAmount(reverse_rate, amount);
    return response->Send(dto::MakeConvertedAmountDtoJson(
                              base_curr->model_, target_curr->model_,
                              reverse_rate.str(2, std::ios::fixed),
                              amount.str(2, std::ios::fixed),
                              converted_amount.str(2, std::ios::fixed))
                              .dump());
  }

  const auto [usd_base, usd_target] = FindUsdExchangeRates(
      base_curr->model_.currency_id, target_curr->model_.currency_id);
  const auto rate = calcs::CalcRateByUsdExchRates(usd_base->model_.rate,
                                                  usd_target->model_.rate);
  const Decimal amount{request_data.amount_};
  const auto converted_amount =
      calcs::CalcConvAmount(rate, amount).str(2, std::ios::fixed);

  response->Send(dto::MakeConvertedAmountDtoJson(
                     base_curr->model_, target_curr->model_,
                     rate.str(2, std::ios::fixed),
                     amount.str(2, std::ios::fixed), converted_amount)
                     .dump());
}

Handler::CurrencyPair Handler::FindCurrencyPairByCodes(
    const std::string& base_curr_code,
    const std::string& target_curr_code) const {
  const auto base_curr = curr_cache_->GetCurrencyByCode(base_curr_code);
  if (!base_curr) {
    throw engine::http_server::HttpException{
        utils::MakeError("Base currency not found"),
        engine::http_server::HttpStatus::kNotFound};
  }
  const auto target_curr = curr_cache_->GetCurrencyByCode(target_curr_code);
  if (!target_curr) {
    throw engine::http_server::HttpException{
        utils::MakeError("Target currency not found"),
        engine::http_server::HttpStatus::kNotFound};
  }
  return {std::move(base_curr), std::move(target_curr)};
}

Handler::ExchangeRates Handler::FindUsdExchangeRates(
    const std::int32_t base_curr_id, const std::int32_t target_curr_id) const {
  const auto usd = curr_cache_->GetCurrencyByCode("USD");
  if (!usd) {
    throw engine::http_server::HttpException{
        utils::MakeError("Exchange rate not found"),
        engine::http_server::HttpStatus::kNotFound};
  }
  const auto usd_base =
      FindExchangeRateByCurrencyPair(usd->model_.currency_id, base_curr_id);
  if (!usd_base) {
    throw engine::http_server::HttpException{
        utils::MakeError("Exchange rate not found"),
        engine::http_server::HttpStatus::kNotFound};
  }
  const auto usd_target =
      FindExchangeRateByCurrencyPair(usd->model_.currency_id, target_curr_id);
  if (!usd_target) {
    throw engine::http_server::HttpException{
        utils::MakeError("Exchange rate not found"),
        engine::http_server::HttpStatus::kNotFound};
  }
  return {std::move(usd_base), std::move(usd_target)};
}

caches::ExchangeRatesCache::ExchangeRateConstPtr
Handler::FindExchangeRateByCurrencyPair(
    const std::int32_t base_curr_id, const std::int32_t target_curr_id) const {
  const auto exch_rate = exch_rates_cache_->GetExchangeRateByCurrPair(
      base_curr_id, target_curr_id);
  return exch_rate;
}

}  // namespace currexch_service::handlers::exchange::get
