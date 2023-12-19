#pragma once

#include <boost/algorithm/string.hpp>
#include <string>
#include "utils/net.hpp"

namespace engine::http_server {

enum class HttpMethod {
  kGet,
  kHead,
  kPost,
  kPut,
  kDelete,
  kConnect,
  kOptions,
  kTrace,
  kPatch,
  kUnknown
};

HttpMethod ToHttpMethodEnum(std::string_view string_http_method);
HttpMethod ToHttpMethodEnum(http::verb beast_http_method);
std::string_view ToString(HttpMethod method);

}  // namespace engine::http_server