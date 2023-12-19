#pragma once

#include <boost/algorithm/string/predicate.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include "boost/assign.hpp"
#include "boost/bimap.hpp"
#include "boost/bimap/unordered_set_of.hpp"
#include "type_traits.hpp"

namespace engine::utils {

template <typename T>
struct IEqualTo {
  bool operator()(const T& lhs, const T& rhs) const {
    static_assert(is_string_type_v<T>, "T must be a string type");
    return boost::algorithm::iequals(lhs, rhs, std::locale());
  }
};

template <typename T>
struct IHash {
  size_t operator()(const T& data) const {
    static_assert(is_string_type_v<T>, "T must be a string type");
    size_t seed = 0;
    std::locale locale;
    for (auto it = data.begin(); it != data.end(); ++it) {
      boost::hash_combine(seed, std::toupper(*it, locale));
    }
    return seed;
  }
};

}  // namespace engine::utils