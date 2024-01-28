#pragma once

#include "wecs/entity/entity.hpp"
#include "wecs/config/config.hpp"
#include "wecs/entity/resource.hpp"

#include <string>

namespace wecs {

/**
 * @brief a help class for create/delete/replace entity/resource/component from
 * basic_registry
 *
 * @tparam WorldT
 */
template <typename WorldT>
class basic_commands final {
public:
    using registry_type = typename WorldT::registry_type;
    using entity_type = typename registry_type::entity_type;

    basic_commands(WorldT& world, registry_type& registry)
        : world_(&world), registry_(&registry) {}

    entity_type create() { return registry_->create(); }

    template <typename Type, typename... Args>
    Type& emplace(entity_type entity, Args&&... args) {
        return registry_->template emplace<Type>(entity, std::forward<Args>(args)...);
    }

    template <typename Type, typename... Args>
    Type& replace(entity_type entity, Args&&... args) {
        return registry_->template replace<Type>(entity, std::forward<Args>(args)...);
    }

    template <typename T>
    void emplace_bundle(entity_type entity, T&& bundle) {
        registry_->emplace_bundle(entity, std::forward<T>(bundle));
    }

    auto& destroy(entity_type entity) {
        registry_->destroy(entity);
        return *this;
    }

    template <typename Type>
    void remove(entity_type entity) {
        registry_->template remove<Type>(entity);
    }

    template <typename Type>
    void remove(entity_type entity, const config::type_info& typr) noexcept {
        registry_->remove(entity, typr);
    }

    bool alive(entity_type entity) const noexcept {
        return registry_->alive(entity);
    }

    template <typename T, typename... Args>
    T& emplace_resource(Args&&... args) noexcept {
        return internal::resource_cache<T>::instance().emplace(std::forward<Args>(args)...);
    }

    template <typename T>
    auto& remove_resource() noexcept {
        internal::resource_cache<T>::instance().remove();
        return *this;
    }

    template <typename T>
    auto& switch_state(T state) {
        registry_->template switch_state<T>(state);
        return *this;
    }

    template <typename T>
    auto& on_construct() noexcept {
        return registry_->template on_construct<T>();
    }

    template <typename T>
    auto& on_update() noexcept {
        return registry_->template on_update<T>();
    }

    template <typename T>
    auto& on_destruct() noexcept {
        return registry_->template on_destruct<T>();
    }

    auto& switch_registry(const std::string& name) noexcept {
        world_->switch_registry(name);
        return *this;
    }

private:
    WorldT* world_;
    registry_type* registry_;
};

}  // namespace wecs