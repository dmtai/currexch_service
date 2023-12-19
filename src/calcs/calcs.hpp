#pragma once

#include <string>
#include "utils/multiprecision.hpp"

namespace currexch_service::calcs {

Decimal CalcConvAmount(const Decimal& rate, const Decimal& amount);
Decimal CalcReverseRate(const Decimal& rate);
Decimal CalcRateByUsdExchRates(std::string_view usd_base_rate,
                               std::string_view usd_target_rate);

}  // namespace currexch_service::calcs