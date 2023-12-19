#pragma once

#include "caches/currencies_cache.hpp"
#include "engine/src/utils/utils.hpp"
#include "engine/src/http_server/http_handler.hpp"
#include "engine/src/http_server/http_request.hpp"
#include "engine/src/http_server/http_response.hpp"
#include "engine/src/postgres/pg_client.hpp"

namespace currexch_service::handlers::exchange_rate::patch {

class Handler : public engine::http_server::HttpHandlerBase {
 public:
  static constexpr std::string_view kName{"patch_exchange_rate"};

  Handler(const engine::YamlConfig& config,
          engine::components::Components& components);

  void HandleRequest(
      const engine::http_server::HttpRequestConstPtr& request,
      const engine::http_server::HttpResponsePtr& response) const override;

 private:
  const engine::postgres::PgClientPtr pg_client_;
  const caches::CurrenciesCachePtr curr_cache_;
};

}  // namespace currexch_service::handlers::exchange_rate::patch