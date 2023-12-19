#pragma once

#include <string>
#include <unordered_map>

namespace engine::utils::form_urlencoded {

using XWWWWFormUrlencodedData = std::unordered_map<std::string, std::string>;

XWWWWFormUrlencodedData ParseXWWWWFormUrlencodedStr(const std::string& str);

}  // namespace engine::utils::form_urlencoded