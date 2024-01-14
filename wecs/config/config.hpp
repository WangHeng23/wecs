#pragma once

#include "wecs/entity/entity.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>

#ifndef ENTITY_NUMERIC_TYPE
#define ENTITY_NUMERIC_TYPE uint32_t
#endif

#ifndef SPARSE_PAGE_SIZE
#define SPARSE_PAGE_SIZE 4096
#endif

namespace wecs {

struct fake_type_info {};

template <typename T>
fake_type_info* get_fake_type_info() {
    static fake_type_info typeinfo;
    return &typeinfo;
}

}

#ifndef WECS_TYPE_INFO_TYPE
#define WECS_TYPE_INFO_TYPE const fake_type_info*
#endif

#ifndef WECS_GET_TYPE_INFO
#define WECS_GET_TYPE_INFO(type) get_fake_type_info<type>()
#endif

namespace wecs::internal {

struct fake_reference_any final {
    template <typename T>
    fake_reference_any(const T&) {}

    fake_reference_any() = default;
};

}

#ifndef WECS_ANY
#define WECS_ANY ::wecs::internal::fake_reference_any
#endif

#ifndef WECS_MAKE_ANY_REF
#define WECS_MAKE_ANY_REF(x)  ::wecs::internal::fake_reference_any(x)
#endif

namespace wecs {

namespace config {

using type_info = WECS_TYPE_INFO_TYPE;

enum class Entity : ENTITY_NUMERIC_TYPE {};
constexpr uint32_t PageSize = SPARSE_PAGE_SIZE;

inline std::ostream& operator<<(std::ostream& o, Entity entity) {
    o << "Ent(" << static_cast<ENTITY_NUMERIC_TYPE>(entity) << ")";
    return o;
}

inline bool operator==(Entity e1, Entity e2) {
    return internal::entity_to_integral(e1) == internal::entity_to_integral(e2);
}

inline bool operator!=(Entity e1, Entity e2) {
    return !(e1 == e2);
}

inline bool operator==(Entity e1, uint64_t num) {
    return internal::entity_to_integral(e1) == num;
}

inline bool operator!=(Entity e1, uint64_t num) {
    return !(e1 == num);
}

inline bool operator==(uint64_t num, Entity e) {
    return e == num;
}

inline bool operator!=(uint64_t num, Entity e) {
    return !(e == num);
}

using id_type = size_t;

}  // namespace config

}  // namespace wecs