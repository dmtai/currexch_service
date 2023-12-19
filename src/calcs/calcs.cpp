#include "calcs.hpp"

namespace currexch_service::calcs {

Decimal CalcConvAmount(const Decimal& rate, const Decimal& amount) {
  return amount * rate;
}

Decimal CalcReverseRate(const Decimal& rate) { return Decimal{1} / rate; }

Decimal CalcRateByUsdExchRates(std::string_view usd_base_rate,
                               std::string_view usd_target_rate) {
  return Decimal{usd_target_rate} / Decimal{usd_base_rate};
}

}  // namespace currexch_service::calcs