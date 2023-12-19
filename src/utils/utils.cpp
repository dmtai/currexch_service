#include "utils.hpp"

namespace currexch_service::utils {

CurrencyPairOpt ParseCurrPair(const std::string& curr_pair) {
  if (curr_pair.size() != 6) {
    return std::nullopt;
  }
  return {{curr_pair.substr(0, 3), curr_pair.substr(3)}};
}

}  // namespace currexch_service::utils