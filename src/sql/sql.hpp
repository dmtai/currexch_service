#pragma once

#include <string_view>

namespace currexch_service::sql {

inline constexpr std::string_view kInsertNewCurrency{R"~(
INSERT INTO 
    curr_exchange.currencies(code, full_name, sign) 
VALUES 
    ($1, $2, $3)
RETURNING 
    currency_id, full_name, code, sign
)~"};

inline constexpr std::string_view kInsertNewExchangeRate{R"~(
INSERT INTO 
    curr_exchange.exchange_rates(base_currency_id, target_currency_id, rate) 
VALUES
    ($1, $2, $3::numeric(8,6)) 
RETURNING 
    exchange_rate_id
)~"};

inline constexpr std::string_view kSelectExchangeRateWithCurrenciesById{R"~(
SELECT 
    exchange_rate_id, 
    base_curr.currency_id as base_currency_id, 
    base_curr.code as base_currency_code, 
    base_curr.full_name as base_currency_full_name, 
    base_curr.sign as base_currency_sign,
    target_curr.currency_id as target_currency_id, 
    target_curr.code as target_currency_code, 
    target_curr.full_name as target_currency_full_name, 
    target_curr.sign as target_currency_sign,
    rate::text
FROM 
    curr_exchange.exchange_rates AS exch_rates
INNER JOIN 
    curr_exchange.currencies AS base_curr 
ON 
    base_curr.currency_id = exch_rates.base_currency_id 
INNER JOIN 
    curr_exchange.currencies AS target_curr 
ON 
    target_curr.currency_id = exch_rates.target_currency_id WHERE exchange_rate_id = $1
)~"};

inline constexpr std::string_view kUpdateExchangeRateByCurrPair{R"~(
UPDATE 
    curr_exchange.exchange_rates AS exch_rates 
SET 
    rate=$1::numeric(8,6) 
WHERE 
    exch_rates.base_currency_id=$2 AND
    exch_rates.target_currency_id=$3
RETURNING 
    exchange_rate_id
)~"};

inline constexpr std::string_view kUpdateCurrenciesCache{R"~(
SELECT 
    currency_id, code, full_name, sign 
FROM 
    curr_exchange.currencies 
WHERE
    updated_at > $1::timestamptz
)~"};

inline constexpr std::string_view kFullUpdateCurrenciesCache{R"~(
SELECT 
    currency_id, code, full_name, sign 
FROM 
    curr_exchange.currencies
)~"};

inline constexpr std::string_view kUpdateExchangeRatesCache{R"~(
SELECT 
    exchange_rate_id, base_currency_id, target_currency_id, rate::text 
FROM 
    curr_exchange.exchange_rates 
WHERE 
    updated_at > $1::timestamptz
)~"};

inline constexpr std::string_view kFullUpdateExchangeRatesCache{R"~(
SELECT 
    exchange_rate_id, base_currency_id, target_currency_id, rate::text
FROM 
    curr_exchange.exchange_rates
)~"};

}  // namespace currexch_service::sql