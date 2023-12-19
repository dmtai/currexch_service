#include "exceptions.hpp"

namespace engine::http_server {

HttpException::HttpException(std::string msg, HttpStatus http_status)
    : std::runtime_error{std::move(msg)}, http_status_{http_status} {}

HttpStatus HttpException::GetHttpStatus() const noexcept {
  return http_status_;
}

}  // namespace engine::http_server