#pragma once

#include "wecs/config/config.hpp"
#include "wecs/entity/commands.hpp"
#include "wecs/entity/querier.hpp"
#include "wecs/entity/resource.hpp"
#include "wecs/entity/registry_wrapper.hpp"
#include "wecs/entity/world.hpp"

namespace wecs {

using world = basic_world<wecs::config::Entity, wecs::config::PageSize>;

template <typename... Ts>
using querier = typename world::registry_type::querier_type<Ts...>;

using commands = typename world::registry_type::commands_type;

template <typename T>
using event_dispatcher = typename world::registry_type::event_dispatcher_wrapper_type<T>;

using registry = registry_wrapper<typename world::registry_type>;

}  // namespace wecs