#pragma once

#include "models/currency.hpp"
#include "models/exchange_rate.hpp"
#include "models/exchange_rate_with_currencies.hpp"

namespace currexch_service::dto {

engine::Json MakeExchangeRateDtoJson(const models::ExchangeRate& exch_rate,
                                     const engine::Json& base_curr,
                                     const engine::Json& target_curr);
engine::Json MakeExchangeRateWithCurrDtoJson(
    const models::ExchangeRateWithCurrencies& exch_rate);
engine::Json MakeCurrencyDtoJson(const models::Currency& curr);
engine::Json MakeConvertedAmountDtoJson(
    const models::Currency& base_curr, const models::Currency& target_curr,
    const std::string& rate_str, const std::string& amount_str,
    const std::string& converted_amount_str);

}  // namespace currexch_service::dto