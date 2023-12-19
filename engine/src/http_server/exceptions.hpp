#pragma once

#include <exception>
#include <string>
#include "http_status.hpp"

namespace engine::http_server {

class HttpException : public std::runtime_error {
 public:
  HttpException(std::string msg, HttpStatus http_status);
  HttpStatus GetHttpStatus() const noexcept;

 private:
  const HttpStatus http_status_;
};

}  // namespace engine::http_server