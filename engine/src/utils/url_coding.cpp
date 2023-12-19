#include "url_coding.hpp"
#include <array>
#include <cstdint>

namespace engine::utils {

char HexCharToBin(char ch) {
  if (ch >= '0' && ch <= '9') return (char)(ch - '0');
  if (ch >= 'a' && ch <= 'f') return (char)(ch - 'a' + 10);
  if (ch >= 'A' && ch <= 'F') return (char)(ch - 'A' + 10);
  return -1;
}

char HexStrToBin(std::string_view str) {
  auto high = HexCharToBin(str[0]);
  auto low = HexCharToBin(str[1]);
  if (high == -1 || low == -1) {
    return -1;
  }
  return (high << 4) | low;
}

std::string UrlEncode(std::string_view str) {
  std::string out;
  for (size_t i = 0; i < str.size(); ++i) {
    char ch = str[i];
    if (std::isalnum((std::uint8_t)ch)) {
      out.push_back(ch);
    } else {
      std::array<char, 4> buf;
      std::sprintf(buf.data(), "%%%X%X", (std::uint8_t)ch >> 4,
                   (std::uint8_t)ch & 0x0F);
      out.append(buf.data());
    }
  }
  return out;
}

std::string UrlDecode(std::string_view str) {
  std::string output;
  size_t i = 0, len = str.size();
  while (i < len) {
    if (str[i] == '%') {
      if (i + 3 > len) {
        output.append(str, i, len - i);
        break;
      }
      const auto ch = HexStrToBin(&(str[i + 1]));
      if (ch == -1) {
        output.append(str, i, 3);
      } else {
        output += ch;
      }
      i += 3;
    } else {
      output += str[i];
      ++i;
    }
  }
  return output;
}

}  // namespace engine::utils