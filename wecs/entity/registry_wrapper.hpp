#pragma once

#include "wecs/config/config.hpp"

namespace wecs {

/**
 * @brief a wrapper for basic_registry
 */
template <typename RegistryT>
class registry_wrapper final {
public:
    using registry_type = RegistryT;
    using entity_type = typename RegistryT::entity_type;

    registry_wrapper(registry_type& registry) : registry_(&registry) {}

    bool alive(entity_type entity) const noexcept {
        return registry_->alive(entity);
    }

    template <typename T>
    bool has(entity_type entity) const noexcept {
        return registry_->template has<T>(entity);
    }

    bool has(entity_type entity,
             const config::type_info& type_info) const noexcept {
        return registry_->has(entity, type_info);
    }

    WECS_ANY get_mut(entity_type entity,
                     const config::type_info& type_info) noexcept {
        return registry_->get_mut(entity, type_info);
    }

    template <typename Type>
    const Type& get(entity_type entity) const noexcept {
        return registry_->template get<Type>(entity);
    }

    auto commands() const noexcept { return registry_->commands(); }

    template <typename Type>
    Type& get_mut(entity_type entity) noexcept {
        return registry_->template get_mut<Type>(entity);
    }

    auto& entities() const noexcept { return registry_->entities(); }

    const auto& typeinfos() const noexcept { return registry_->typeinfos(); }

    const auto& pools() const noexcept { return registry_->pools(); }

    template <typename T>
    auto res() noexcept {
        return registry_->template res<T>();
    }

    template <typename T>
    auto res_mut() noexcept {
        return registry_->template res_mut<T>();
    }

    template <typename T>
    void call_system() {
        registry_->template call_system<T>();
    }

    template <typename T>
    auto construct_system() {
        return registry_->template construct_system<T>();
    }

    template <typename T>
    auto event_dispatcher() {
        return registry_->template event_dispatcher<T>();
    }

private:
    registry_type* registry_;
};

}  // namespace wecs