#pragma once

#include <boost/hana/adapt_struct.hpp>
#include <memory>
#include <vector>
#include "engine/src/utils/json.hpp"
#include "types.hpp"

namespace currexch_service::models {

struct Currency {
  std::int32_t currency_id;
  varchar code;
  varchar full_name;
  varchar sign;
};

using CurrencyPtr = std::shared_ptr<Currency>;
using CurrencyConstPtr = std::shared_ptr<const Currency>;
using CurrenciesVec = std::vector<Currency>;

}  // namespace currexch_service::models

BOOST_HANA_ADAPT_STRUCT(currexch_service::models::Currency, currency_id, code,
                        full_name, sign);