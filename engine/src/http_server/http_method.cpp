#include "http_method.hpp"
#include "utils/bimap.hpp"

namespace engine::http_server {

namespace {

namespace http_methods {

constexpr std::string_view kGet{"get"};
constexpr std::string_view kHead{"head"};
constexpr std::string_view kPost{"post"};
constexpr std::string_view kPut{"put"};
constexpr std::string_view kDelete{"delete"};
constexpr std::string_view kConnect{"connect"};
constexpr std::string_view kOptions{"options"};
constexpr std::string_view kTrace{"trace"};
constexpr std::string_view kPatch{"patch"};

}  // namespace http_methods

using HttpMethodEnumByString =
    utils::CaseInsensitiveUnorderedBiMap<std::string_view, HttpMethod>;

const auto http_method_enum_by_string =
    utils::MakeBimap<HttpMethodEnumByString>(
        {{http_methods::kGet, HttpMethod::kGet},
         {http_methods::kHead, HttpMethod::kHead},
         {http_methods::kPost, HttpMethod::kPost},
         {http_methods::kPut, HttpMethod::kPut},
         {http_methods::kDelete, HttpMethod::kDelete},
         {http_methods::kConnect, HttpMethod::kConnect},
         {http_methods::kOptions, HttpMethod::kOptions},
         {http_methods::kTrace, HttpMethod::kTrace},
         {http_methods::kPatch, HttpMethod::kPatch}});

using HttpMethodByBeastVerb = utils::UnorderedBiMap<http::verb, HttpMethod>;

const auto http_method_by_beast_verb = utils::MakeBimap<HttpMethodByBeastVerb>({
    {http::verb::get, HttpMethod::kGet},
    {http::verb::head, HttpMethod::kHead},
    {http::verb::post, HttpMethod::kPost},
    {http::verb::put, HttpMethod::kPut},
    {http::verb::delete_, HttpMethod::kDelete},
    {http::verb::connect, HttpMethod::kConnect},
    {http::verb::options, HttpMethod::kOptions},
    {http::verb::trace, HttpMethod::kTrace},
    {http::verb::patch, HttpMethod::kPatch},
});

}  // namespace

HttpMethod ToHttpMethodEnum(std::string_view string_http_method) {
  const auto it = http_method_enum_by_string.left.find(string_http_method);
  if (it != http_method_enum_by_string.left.end()) {
    return it->second;
  }
  return HttpMethod::kUnknown;
}

HttpMethod ToHttpMethodEnum(http::verb beast_http_method) {
  const auto it = http_method_by_beast_verb.left.find(beast_http_method);
  if (it != http_method_by_beast_verb.left.end()) {
    return it->second;
  }
  return HttpMethod::kUnknown;
}

std::string_view ToString(HttpMethod method) {
  const auto it = http_method_enum_by_string.right.find(method);
  if (it != http_method_enum_by_string.right.end()) {
    return it->second;
  }
  return "";
}

}  // namespace engine::http_server