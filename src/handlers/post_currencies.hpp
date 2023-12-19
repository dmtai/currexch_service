#pragma once

#include "engine/src/utils/utils.hpp"
#include "engine/src/http_server/http_handler.hpp"
#include "engine/src/http_server/http_request.hpp"
#include "engine/src/http_server/http_response.hpp"
#include "engine/src/postgres/pg_client.hpp"

namespace currexch_service::handlers::currencies::post {

class Handler : public engine::http_server::HttpHandlerBase {
 public:
  static constexpr std::string_view kName{"post_currencies"};

  Handler(const engine::YamlConfig& config,
          engine::components::Components& components);

  void HandleRequest(
      const engine::http_server::HttpRequestConstPtr& request,
      const engine::http_server::HttpResponsePtr& response) const override;

 private:
  const engine::postgres::PgClientPtr pg_client_;
};

}  // namespace currexch_service::handlers::currencies::post