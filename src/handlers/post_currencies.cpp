#include "post_currencies.hpp"
#include <boost/algorithm/string.hpp>
#include "dto/dto.hpp"
#include "engine/src/http_server/exceptions.hpp"
#include "engine/src/http_server/http_status.hpp"
#include "engine/src/utils/x_www_form_urlencoded.hpp"
#include "models/currency.hpp"
#include "sql/sql.hpp"
#include "utils/make_error.hpp"

namespace currexch_service::handlers::currencies::post {

namespace {

struct RequestData {
  std::string code_;
  std::string full_name_;
  std::string sign_;
};

RequestData ParseRequest(
    engine::utils::form_urlencoded::XWWWWFormUrlencodedData& data) {
  const auto full_name = data.find("name");
  if (full_name == data.end() || full_name->second.empty()) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'name' is required"),
        engine::http_server::HttpStatus::kBadRequest};
  }
  if (full_name->second.size() > 255) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'name' is invalid"),
        engine::http_server::HttpStatus::kBadRequest};
  }

  const auto code = data.find("code");
  if (code == data.end() || code->second.empty()) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'code' is required"),
        engine::http_server::HttpStatus::kBadRequest};
  }
  if (code->second.size() > 3) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'code' is invalid"),
        engine::http_server::HttpStatus::kBadRequest};
  }

  const auto sign = data.find("sign");
  if (sign == data.end() || sign->second.empty()) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'sign' is required"),
        engine::http_server::HttpStatus::kBadRequest};
  }
  if (sign->second.size() > 16) {
    throw engine::http_server::HttpException{
        utils::MakeError("Field 'sign' is invalid"),
        engine::http_server::HttpStatus::kBadRequest};
  }

  boost::algorithm::to_upper(code->second);
  return {std::move(code->second), std::move(full_name->second),
          std::move(sign->second)};
}

}  // namespace

Handler::Handler([[maybe_unused]] const engine::YamlConfig& config,
                 engine::components::Components& components)
    : pg_client_{components.Get<engine::postgres::PgClient>()} {}

void Handler::HandleRequest(
    [[maybe_unused]] const engine::http_server::HttpRequestConstPtr& request,
    const engine::http_server::HttpResponsePtr& response) const {
  auto body = engine::utils::form_urlencoded::ParseXWWWWFormUrlencodedStr(
      request->GetBody());
  const auto request_data = ParseRequest(body);

  const auto query_handler = [response](const auto& ec, const auto& conn,
                                        const auto& curr) {
    if (ec) {
      if (ec == ozo::sqlstate::code::unique_violation) {
        response->SetStatusCode(engine::http_server::HttpStatus::kConflict);
        return response->Send(utils::MakeError("Currency already exists"));
      }
      response->SetStatusCode(
          engine::http_server::HttpStatus::kInternalServerError);
      response->Send(utils::MakeError("DB error"));
    }
    if (curr->size() != 1) {
      response->SetStatusCode(
          engine::http_server::HttpStatus::kInternalServerError);
      return response->Send(utils::MakeError("DB error"));
    }
    response->Send(dto::MakeCurrencyDtoJson((*curr)[0]).dump());
  };

  pg_client_->Execute<models::CurrenciesVec>(
      ozo::make_query(sql::kInsertNewCurrency, request_data.code_,
                      request_data.full_name_, request_data.sign_),
      query_handler);
}

}  // namespace currexch_service::handlers::currencies::post