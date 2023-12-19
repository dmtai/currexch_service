#pragma once

#include <boost/hana/adapt_struct.hpp>
#include <memory>
#include "engine/src/utils/json.hpp"
#include "types.hpp"

namespace currexch_service::models {

struct ExchangeRate {
  std::int32_t exchange_rate_id;
  std::int32_t base_currency_id;
  std::int32_t target_currency_id;
  std::string rate;
};

using ExchangeRatePtr = std::shared_ptr<ExchangeRate>;
using ExchangeRateConstPtr = std::shared_ptr<const ExchangeRate>;

}  // namespace currexch_service::models

BOOST_HANA_ADAPT_STRUCT(currexch_service::models::ExchangeRate,
                        exchange_rate_id, base_currency_id, target_currency_id,
                        rate);