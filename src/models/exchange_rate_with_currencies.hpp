#pragma once

#include <boost/hana/adapt_struct.hpp>
#include <memory>
#include "engine/src/utils/json.hpp"
#include "types.hpp"

namespace currexch_service::models {

struct ExchangeRateWithCurrencies {
  std::int32_t exchange_rate_id;
  std::int32_t base_currency_id;
  varchar base_currency_code;
  varchar base_currency_full_name;
  varchar base_currency_sign;
  std::int32_t target_currency_id;
  varchar target_currency_code;
  varchar target_currency_full_name;
  varchar target_currency_sign;
  std::string rate;
};

using ExchangeRateWithCurrenciesPtr =
    std::shared_ptr<ExchangeRateWithCurrencies>;
using ExchangeRateWithCurrenciesConstPtr =
    std::shared_ptr<const ExchangeRateWithCurrencies>;
using ExchangeRateWithCurrenciesVec = std::vector<ExchangeRateWithCurrencies>;

}  // namespace currexch_service::models

BOOST_HANA_ADAPT_STRUCT(currexch_service::models::ExchangeRateWithCurrencies,
                        exchange_rate_id, base_currency_id, base_currency_code,
                        base_currency_full_name, base_currency_sign,
                        target_currency_id, target_currency_code,
                        target_currency_full_name, target_currency_sign, rate);