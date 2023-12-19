#include "dto.hpp"
#include <bitset>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include "engine/src/utils/utils.hpp"

namespace currexch_service::dto {

engine::Json MakeExchangeRateDtoJson(const models::ExchangeRate& exch_rate,
                                     const engine::Json& base_curr,
                                     const engine::Json& target_curr) {
  engine::Json json;
  json["id"] = exch_rate.exchange_rate_id;
  json["baseCurrency"] = base_curr;
  json["targetCurrency"] = target_curr;
  json["rate"] = engine::utils::FromChars<double>(
      boost::multiprecision::cpp_dec_float_50{exch_rate.rate}.str(
          2, std::ios::fixed));
  return json;
}

engine::Json MakeExchangeRateWithCurrDtoJson(
    const models::ExchangeRateWithCurrencies& exch_rate_with_curr) {
  engine::Json exch_rate_json;
  exch_rate_json["id"] = exch_rate_with_curr.exchange_rate_id;

  engine::Json base_curr_json;
  base_curr_json["currency_id"] = exch_rate_with_curr.base_currency_id;
  base_curr_json["code"] =
      static_cast<std::string>(exch_rate_with_curr.base_currency_code);
  base_curr_json["full_name"] =
      static_cast<std::string>(exch_rate_with_curr.base_currency_full_name);
  base_curr_json["sign"] =
      static_cast<std::string>(exch_rate_with_curr.base_currency_sign);

  engine::Json target_curr_json;
  target_curr_json["currency_id"] = exch_rate_with_curr.target_currency_id;
  target_curr_json["code"] =
      static_cast<std::string>(exch_rate_with_curr.target_currency_code);
  target_curr_json["full_name"] =
      static_cast<std::string>(exch_rate_with_curr.target_currency_full_name);
  target_curr_json["sign"] =
      static_cast<std::string>(exch_rate_with_curr.target_currency_sign);

  exch_rate_json["baseCurrency"] = std::move(base_curr_json);
  exch_rate_json["targetCurrency"] = std::move(target_curr_json);

  exch_rate_json["rate"] = engine::utils::FromChars<double>(
      boost::multiprecision::cpp_dec_float_50{exch_rate_with_curr.rate}.str(
          2, std::ios::fixed));
  return exch_rate_json;
}

engine::Json MakeConvertedAmountDtoJson(
    const models::Currency& base_curr, const models::Currency& target_curr,
    const std::string& rate_str, const std::string& amount_str,
    const std::string& converted_amount_str) {
  engine::Json exch_rate_json;

  engine::Json base_curr_json;
  base_curr_json["currency_id"] = base_curr.currency_id;
  base_curr_json["code"] = static_cast<std::string>(base_curr.code);
  base_curr_json["full_name"] = static_cast<std::string>(base_curr.full_name);
  base_curr_json["sign"] = static_cast<std::string>(base_curr.sign);

  engine::Json target_curr_json;
  target_curr_json["currency_id"] = target_curr.currency_id;
  target_curr_json["code"] = static_cast<std::string>(target_curr.code);
  target_curr_json["full_name"] =
      static_cast<std::string>(target_curr.full_name);
  target_curr_json["sign"] = static_cast<std::string>(target_curr.sign);

  exch_rate_json["baseCurrency"] = std::move(base_curr_json);
  exch_rate_json["targetCurrency"] = std::move(target_curr_json);

  exch_rate_json["rate"] = engine::utils::FromChars<double>(rate_str);
  exch_rate_json["amount"] = engine::utils::FromChars<double>(amount_str);
  exch_rate_json["convertedAmount"] =
      engine::utils::FromChars<double>(converted_amount_str);

  return exch_rate_json;
}

engine::Json MakeCurrencyDtoJson(const models::Currency& curr) {
  engine::Json json;
  json["currency_id"] = curr.currency_id;
  json["code"] = static_cast<std::string>(curr.code);
  json["full_name"] = static_cast<std::string>(curr.full_name);
  json["sign"] = static_cast<std::string>(curr.sign);
  return json;
}

}  // namespace currexch_service::dto