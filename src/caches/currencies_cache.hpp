#pragma once

#include <libcuckoo/cuckoohash_map.hh>
#include <unordered_map>
#include "engine/src/utils/utils.hpp"
#include "engine/src/postgres/pg_cache.hpp"
#include "engine/src/utils/case_insensitive_hash.hpp"
#include "models/currency.hpp"
#include "sql/sql.hpp"

namespace currexch_service::caches {

class CurrenciesCache
    : public engine::postgres::PgCache<CurrenciesCache, models::Currency> {
 public:
  struct Currency {
    models::Currency model_;
    engine::Json dto_json_;
  };

  using CurrencyConstPtr = std::shared_ptr<const Currency>;
  using AllCurrenciesJsonConstPtr = std::shared_ptr<const std::string>;
  using AllCurrenciesJsonAtomicConstPtr =
      std::atomic<AllCurrenciesJsonConstPtr>;
  using CurrencyById =
      libcuckoo::cuckoohash_map<std::int32_t, CurrencyConstPtr>;
  using CurrencyByCode =
      libcuckoo::cuckoohash_map<std::string, CurrencyConstPtr,
                                engine::utils::IHash<std::string>,
                                engine::utils::IEqualTo<std::string>>;
  using CurrencyByIdStdMap = std::unordered_map<std::int32_t, CurrencyConstPtr>;

  static constexpr std::string_view kName{"currencies_cache"};
  static constexpr std::string_view kUpdateQuery{sql::kUpdateCurrenciesCache};
  static constexpr std::string_view kFullUpdateQuery{
      sql::kFullUpdateCurrenciesCache};

  CurrenciesCache(const engine::YamlConfig& config,
                  engine::components::Components& components);

  void FullUpdateCache(const Rows& rows) override;
  void UpdateCache(const Rows& rows) override;
  AllCurrenciesJsonConstPtr GetAllCurrenciesJson() const noexcept;
  CurrencyConstPtr GetCurrencyById(std::int32_t id) const;
  CurrencyConstPtr GetCurrencyByCode(const std::string& code) const;

 private:
  AllCurrenciesJsonConstPtr AllCurrenciesToJson() const;

  const engine::postgres::PgClientPtr pg_client_;
  CurrencyById currency_by_id_;
  CurrencyByCode currency_by_code_;
  AllCurrenciesJsonAtomicConstPtr all_currencies_json_;
  CurrencyByIdStdMap currency_by_id_std_map_;
};

using CurrenciesCachePtr = std::shared_ptr<CurrenciesCache>;

}  // namespace currexch_service::caches