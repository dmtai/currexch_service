#pragma once

#include <string>

namespace engine::utils {

char HexCharToBin(char ch);
char HexStrToBin(std::string_view str);
std::string UrlEncode(std::string_view str);
std::string UrlDecode(std::string_view str);

}  // namespace engine::utils