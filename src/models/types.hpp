#pragma once

#include <ozo/pg/definitions.h>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <string>

OZO_STRONG_TYPEDEF(std::string, varchar);
OZO_PG_BIND_TYPE(varchar, "varchar");