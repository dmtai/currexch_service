#include "currencies_cache.hpp"
#include "dto/dto.hpp"

namespace currexch_service::caches {

CurrenciesCache::CurrenciesCache(const engine::YamlConfig& config,
                                 engine::components::Components& components)
    : PgCache<CurrenciesCache, models::Currency>(
          config, components.Get<engine::components::ServerIOContext>(),
          components.Get<engine::components::ServerIOContext>()->MakeStrand(),
          components.Get<engine::postgres::PgClient>()),
      pg_client_{components.Get<engine::postgres::PgClient>()},
      all_currencies_json_{std::make_shared<std::string>()} {}

void CurrenciesCache::FullUpdateCache(const Rows& rows) {
  const auto rows_size = rows.size();
  CurrencyById currency_by_id{rows_size};
  CurrencyByCode currency_by_code{rows_size};
  engine::Json all_currencies_json;
  currency_by_id_std_map_.clear();

  for (const auto& row : rows) {
    engine::Json dto_json = dto::MakeCurrencyDtoJson(row);
    auto row_ptr = std::make_shared<const Currency>(std::move(row), dto_json);
    currency_by_id.insert(row_ptr->model_.currency_id, row_ptr);
    currency_by_code.insert(static_cast<std::string>(row_ptr->model_.code),
                            row_ptr);
    currency_by_id_std_map_.insert(
        {row_ptr->model_.currency_id, std::move(row_ptr)});
    all_currencies_json.push_back(std::move(dto_json));
  }
  currency_by_id_ = std::move(currency_by_id);
  currency_by_code_ = std::move(currency_by_code);
  
  all_currencies_json_.exchange(std::make_shared<std::string>(
      all_currencies_json.empty() ? "" : all_currencies_json.dump()));
}

void CurrenciesCache::UpdateCache(const Rows& rows) {
  if (rows.empty()) {
    return;
  }
  for (const auto& row : rows) {
    auto row_ptr = std::make_shared<const Currency>(
        std::move(row), dto::MakeCurrencyDtoJson(row));
    currency_by_id_.insert_or_assign(row_ptr->model_.currency_id, row_ptr);
    currency_by_code_.insert_or_assign(
        static_cast<std::string>(row_ptr->model_.code), row_ptr);
    currency_by_id_std_map_.insert_or_assign(row_ptr->model_.currency_id,
                                             std::move(row_ptr));
  }
  all_currencies_json_.exchange(AllCurrenciesToJson());
}

CurrenciesCache::AllCurrenciesJsonConstPtr
CurrenciesCache::GetAllCurrenciesJson() const noexcept {
  return all_currencies_json_.load();
}

CurrenciesCache::CurrencyConstPtr CurrenciesCache::GetCurrencyById(
    std::int32_t id) const {
  try {
    return currency_by_id_.find(id);
  } catch ([[maybe_unused]] const std::out_of_range& ex) {
    return nullptr;
  }
}

CurrenciesCache::CurrencyConstPtr CurrenciesCache::GetCurrencyByCode(
    const std::string& code) const {
  try {
    return currency_by_code_.find(code);
  } catch ([[maybe_unused]] const std::out_of_range& ex) {
    return nullptr;
  }
}

CurrenciesCache::AllCurrenciesJsonConstPtr
CurrenciesCache::AllCurrenciesToJson() const {
  if (currency_by_id_std_map_.empty()) {
    return std::make_shared<std::string>();
  }
  engine::Json all_currencies_json;
  for (const auto& currency : currency_by_id_std_map_) {
    all_currencies_json.push_back(currency.second->dto_json_);
  }
  return std::make_shared<std::string>(all_currencies_json.dump());
}

}  // namespace currexch_service::caches
