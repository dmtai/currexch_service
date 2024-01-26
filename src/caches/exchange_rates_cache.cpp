#include "exchange_rates_cache.hpp"
#include "dto/dto.hpp"

namespace currexch_service::caches {

ExchangeRatesCache::ExchangeRatesCache(
    const engine::YamlConfig& config,
    engine::components::Components& components)
    : PgCache<ExchangeRatesCache, models::ExchangeRate>(
          config, components.Get<engine::components::ServerIOContext>(),
          components.Get<engine::components::ServerIOContext>()->MakeStrand(),
          components.Get<engine::postgres::PgClient>()),
      pg_client_{components.Get<engine::postgres::PgClient>()},
      curr_cache_{components.Get<currexch_service::caches::CurrenciesCache>()},
      all_exchange_rates_{std::make_shared<std::string>()},
      has_skipped_exch_rates_{false} {}

void ExchangeRatesCache::FullUpdateCache(const Rows& rows) {
  ExchangeRateById exchange_rate_by_id;
  ExchangeRateByCurrPair exchange_rate_by_curr_pair;
  exchange_rate_by_id_std_map_.clear();

  for (const auto& row : rows) {
    auto row_ptr = std::make_shared<const ExchangeRate>(std::move(row));
    exchange_rate_by_id.insert(row_ptr->model_.exchange_rate_id, row_ptr);
    exchange_rate_by_curr_pair.insert(
        CurrencyPair{row_ptr->model_.base_currency_id,
                     row_ptr->model_.target_currency_id},
        row_ptr);
    exchange_rate_by_id_std_map_.insert(
        {row_ptr->model_.exchange_rate_id, std::move(row_ptr)});
  }
  exchange_rate_by_id_ = std::move(exchange_rate_by_id);
  exchange_rate_by_curr_pair_ = std::move(exchange_rate_by_curr_pair);
  all_exchange_rates_.exchange(AllExchangeRatesToJson(true));
}

void ExchangeRatesCache::UpdateCache(const Rows& rows) {
  curr_cache_->WaitForInit();
  if (rows.empty()) {
    all_exchange_rates_.exchange(AllExchangeRatesToJson(false));
    return;
  }
  for (const auto& row : rows) {
    auto row_ptr = std::make_shared<const ExchangeRate>(std::move(row));
    exchange_rate_by_id_.insert_or_assign(row_ptr->model_.exchange_rate_id,
                                          row_ptr);
    exchange_rate_by_curr_pair_.insert_or_assign(
        CurrencyPair{row_ptr->model_.base_currency_id,
                     row_ptr->model_.target_currency_id},
        row_ptr);
    exchange_rate_by_id_std_map_.insert_or_assign(
        row_ptr->model_.exchange_rate_id, std::move(row_ptr));
  }
  all_exchange_rates_.exchange(AllExchangeRatesToJson(true));
}

ExchangeRatesCache::AllExchangeRatesJsonConstPtr
ExchangeRatesCache::GetAllExchangeRatesJson() const noexcept {
  return all_exchange_rates_.load();
}

ExchangeRatesCache::ExchangeRateConstPtr
ExchangeRatesCache::GetExchangeRateByCurrPair(
    std::int32_t base_curr_id, std::int32_t target_curr_id) const {
  try {
    const auto exch_rate = exchange_rate_by_curr_pair_.find(
        CurrencyPair{base_curr_id, target_curr_id});
    return exch_rate;
  } catch ([[maybe_unused]] const std::out_of_range& ex) {
    return nullptr;
  }
}

ExchangeRatesCache::AllExchangeRatesJsonConstPtr
ExchangeRatesCache::AllExchangeRatesToJson(bool is_exch_rates_updated) {
  if (exchange_rate_by_id_std_map_.empty()) {
    return std::make_shared<std::string>();
  }
  if (!has_skipped_exch_rates_ && !is_exch_rates_updated) {
    return GetAllExchangeRatesJson();
  }
  has_skipped_exch_rates_ = false;
  engine::Json all_exchange_rates;
  for (const auto& exch_rate : exchange_rate_by_id_std_map_) {
    const auto base_curr =
        curr_cache_->GetCurrencyById(exch_rate.second->model_.base_currency_id);
    if (!base_curr) {
      has_skipped_exch_rates_ = true;
      continue;
    }
    const auto target_curr = curr_cache_->GetCurrencyById(
        exch_rate.second->model_.target_currency_id);
    if (!target_curr) {
      has_skipped_exch_rates_ = true;
      continue;
    }
    const auto res = dto::MakeExchangeRateDtoJson(
        exch_rate.second->model_, base_curr->dto_json_, target_curr->dto_json_);
    all_exchange_rates.push_back(res);
  }
  return std::make_shared<std::string>(all_exchange_rates.dump());
}

}  // namespace currexch_service::caches
