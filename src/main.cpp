#include "engine/src/utils/utils.hpp"
#include "engine/src/utils/main.hpp"
#include "engine/src/components/components.hpp"
#include "engine/src/postgres/pg_client.hpp"
#include "caches/currencies_cache.hpp"
#include "caches/exchange_rates_cache.hpp"
#include "handlers/get_currencies.hpp"
#include "handlers/get_currency.hpp"
#include "handlers/get_exchange.hpp"
#include "handlers/get_exchange_rate.hpp"
#include "handlers/get_exchange_rates.hpp"
#include "handlers/patch_exchange_rate.hpp"
#include "handlers/post_currencies.hpp"
#include "handlers/post_exchange_rates.hpp"

int main(int argc, char* argv[]) {
  using namespace currexch_service;

  engine::utils::Main main{argc, argv};
  main.AddComponent<engine::components::ServerIOContext>()
      .AddComponent<engine::postgres::PgClient>()
      .AddComponent<caches::CurrenciesCache>()
      .AddComponent<caches::ExchangeRatesCache>()
      .AddHandler<handlers::currencies::get::Handler>()
      .AddHandler<handlers::currency::get::Handler>()
      .AddHandler<handlers::exchange_rates::get::Handler>()
      .AddHandler<handlers::exchange_rate::get::Handler>()
      .AddHandler<handlers::currencies::post::Handler>()
      .AddHandler<handlers::exchange_rates::post::Handler>()
      .AddHandler<handlers::exchange_rate::patch::Handler>()
      .AddHandler<handlers::exchange::get::Handler>();
  return main.Run();
}