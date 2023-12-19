#pragma once

#include <string>
#include <type_traits>

namespace engine::utils {

template <typename T>
inline constexpr bool is_string_type_v =
    std::is_convertible_v<T, std::string_view>;

}  // namespace engine::utils