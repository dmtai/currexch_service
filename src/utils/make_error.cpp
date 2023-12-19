#include "make_error.hpp"

namespace currexch_service::utils {

std::string MakeError(std::string_view msg) {
  engine::Json json;
  json["msg"] = msg;
  return json.dump();
}

}  // namespace currexch_service::utils