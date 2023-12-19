#pragma once

#include <optional>
#include <string>

namespace currexch_service::utils {

using CurrencyPairOpt = std::optional<std::pair<std::string, std::string>>;

CurrencyPairOpt ParseCurrPair(const std::string& curr_pair);

}  // namespace currexch_service::utils