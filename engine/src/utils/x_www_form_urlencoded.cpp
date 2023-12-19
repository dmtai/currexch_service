#include "x_www_form_urlencoded.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <vector>
#include "url_coding.hpp"

namespace engine::utils::form_urlencoded {

XWWWWFormUrlencodedData ParseXWWWWFormUrlencodedStr(const std::string& str) {
  if (str.empty()) {
    return {};
  }
  std::vector<std::string> pairs;
  boost::split(pairs, str, boost::is_any_of("\\&"));
  if (pairs.empty()) {
    throw std::runtime_error{
        "Error while parsing x-www-form-urlencoded string"};
  }

  std::unordered_map<std::string, std::string> fields;
  for (const auto& pair : pairs) {
    std::vector<std::string> key_value;
    boost::split(key_value, pair, boost::is_any_of("="));
    const auto size = key_value.size();
    if (size == 0 || size > 2) {
      throw std::runtime_error{
          "Error while parsing x-www-form-urlencoded string"};
    }
    fields.insert(
        {UrlDecode(key_value[0]), size == 2 ? UrlDecode(key_value[1]) : ""});
  }
  return fields;
}

}  // namespace engine::utils::form_urlencoded