#include "utils.hpp"
#include <chrono>

namespace engine::utils {

std::string GetSystemTimeWithTimezone() {
  static const auto kTimezone = std::chrono::current_zone();
  std::chrono::zoned_time zt{kTimezone, std::chrono::system_clock::now()};
  return (std::ostringstream{} << zt).str();
}

http::file_body::value_type OpenFileBody(const std::filesystem::path& path) {
  if (!std::filesystem::exists(path)) {
    throw std::runtime_error(
        fmt::format("File {} doesn't exist", path.generic_string()));
  }

  http::file_body::value_type file_body;
  boost::system::error_code ec;
  const auto path_str = path.generic_string();
  file_body.open(path_str.c_str(), beast::file_mode::read, ec);
  if (ec) {
    throw std::runtime_error(fmt::format("Error opening file \"{}\". Error: {}",
                                         path_str, ec.value()));
  }
  return file_body;
}

}  // namespace engine::utils