#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include "components/components.hpp"
#include "config.hpp"
#include "http_filter.hpp"
#include "http_handler.hpp"
#include "utils/factory.hpp"

namespace engine::http_server {

class HttpHandlersFactory
    : public utils::FactoryBase<HttpHandlerBase, const YamlConfig&,
                                components::Components&> {};
class HttpFiltersFactory
    : public utils::FactoryBase<HttpFilterBase, const YamlConfig&,
                                components::Components&> {};

}  // namespace engine::http_server