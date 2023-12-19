#pragma once

#include <boost/multiprecision/cpp_dec_float.hpp>

namespace currexch_service {

namespace mp = boost::multiprecision;
using Decimal = mp::cpp_dec_float_50;

}  // namespace currexch_service