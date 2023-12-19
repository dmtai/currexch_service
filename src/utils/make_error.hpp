#pragma once

#include <string>
#include "engine/src/utils/json.hpp"

namespace currexch_service::utils {

std::string MakeError(std::string_view msg);

}  // namespace currexch_service::utils