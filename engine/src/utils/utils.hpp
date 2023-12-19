#pragma once

#include <fmt/core.h>
#include <charconv>
#include <filesystem>
#include <optional>
#include <string>
#include "net.hpp"

namespace engine::utils {

std::string GetSystemTimeWithTimezone();
http::file_body::value_type OpenFileBody(const std::filesystem::path& path);

template <typename T,
          std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
T FromChars(std::string_view str) {
  T value{};
  const auto result =
      std::from_chars(str.data(), str.data() + str.size(), value);
  if (result.ec != std::errc()) {
    throw std::runtime_error{"FromChars error"};
  }
  return value;
}

}  // namespace engine::utils