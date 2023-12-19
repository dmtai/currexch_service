#pragma once

#include <boost/algorithm/string/predicate.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include "boost/assign.hpp"
#include "boost/bimap.hpp"
#include "boost/bimap/unordered_set_of.hpp"
#include "case_insensitive_hash.hpp"
#include "type_traits.hpp"

namespace engine::utils {

namespace bimaps = boost::bimaps;

template <typename KeyType, typename ValueType>
using UnorderedBiMap = bimaps::bimap<bimaps::unordered_set_of<KeyType>,
                                     bimaps::unordered_set_of<ValueType>>;

template <typename KeyType, typename ValueType>
using CaseInsensitiveUnorderedBiMap = bimaps::bimap<
    bimaps::unordered_set_of<KeyType, IHash<KeyType>, IEqualTo<KeyType>>,
    bimaps::unordered_set_of<ValueType>>;

template <typename Bimap>
Bimap MakeBimap(std::initializer_list<typename Bimap::value_type> list) {
  return Bimap(list.begin(), list.end());
}

}  // namespace engine::utils