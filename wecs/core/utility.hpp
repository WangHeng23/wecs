#pragma once

#include <cassert>
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace wecs {

#ifndef WECS_ASSERT
#define WECS_ASSERT(expr, msg) assert(((void)msg, expr))
#endif

/**
 * @brief Checks whether a value is a power of two or not (waiting for C++20 and
 * `std::has_single_bit`).
 * @param value A value that may or may not be a power of two.
 * @return True if the value is a power of two, false otherwise.
 */
[[nodiscard]] inline constexpr bool is_power_of_two(const std::size_t value) noexcept {
    return value && (((value - 1) & value) == 0);
}

/**
 * @brief Fast module utility function (powers of two only).
 * @param value A value for which to calculate the modulus.
 * @param mod _Modulus_, it must be a power of two.
 * @return The common remainder.
 */
template <typename T>
constexpr T fast_mod(T value, T mod) {
    static_assert(std::is_integral_v<T> && std::is_unsigned_v<T> && is_power_of_two(mod));
    return value & (mod - 1u);
}

//! @brief a small help class to show type info when compile
//! @tparam T
template <typename T>
struct show_tmpl_error;

/**
 * @brief Transcribes the constness of a type to another type.
 * @tparam To: The type to which to transcribe the constness.
 * @tparam From: The type from which to transcribe the constness.
 */
template <typename To, typename From>
struct constness_as {
    using type = std::remove_const_t<To>;
};

//! @copydoc constness_as
template <typename To, typename From>
struct constness_as<To, const From> {
    using type = const To;
};

//! @brief Alias template to facilitate the transcription of the constness.
template <typename To, typename From>
using constness_as_t = typename constness_as<To, From>::type;

//! @brief function_pointer to function_type
template <typename T>
struct strip_function_pointer_to_type;

template <typename Ret, typename... Args>
struct strip_function_pointer_to_type<Ret (*)(Args...)> {
    using type = Ret(Args...);
};

template <typename Ret, typename... Args>
struct strip_function_pointer_to_type<Ret(Args...)> {
    using type = Ret(Args...);
};

//! @brief Alias template to facilitate the transcription of the function pointer
template <typename T>
using strip_function_pointer_to_type_t = typename strip_function_pointer_to_type<T>::type;

//! @brief Add N to each element in a std::index_sequence
template <std::size_t N, std::size_t... Seq>
constexpr std::index_sequence<N + Seq...> add(std::index_sequence<Seq...>) {
    return {};
}

//! @brief make a std::index_sequence from a give range
//! @tparam Min
//! @tparam Max
template <std::size_t Min, std::size_t Max>
using make_index_range = decltype(add<Min>(std::make_index_sequence<Max - Min>()));

namespace internal {

// a way to extract members of POD into tuple
// reference:
// https://www.reddit.com/r/cpp/comments/4yp7fv/c17_structured_bindings_convert_struct_to_a_tuple/

struct any_type {
    template <typename T>
    constexpr operator T();
};

// reference:
// https://stackoverflow.com/questions/20885541/is-braces-constructible-type-trait
template <typename T, typename... Ts>
decltype(void(T{std::declval<Ts>()...}), std::true_type())
is_braces_constructible_impl(int);

template <typename T, typename... Ts>
std::false_type 
is_braces_constructible_impl(...);

template <typename T, typename... Ts>
struct is_braces_constructible : decltype(is_braces_constructible_impl<T, Ts...>(0)) {};

}  // namespace internal

template <typename T, typename... Ts>
constexpr bool is_braces_constructible_v = internal::is_braces_constructible<T, Ts...>::value;

template <typename T>
auto extract_pod_members(T&& obj) {
    using type = std::decay_t<T>;
    using t = internal::any_type;

    if constexpr (is_braces_constructible_v<type, t, t, t, t, t, t, t, t, t, t>) {
        auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p10] = obj;
        return std::make_tuple(p1, p2, p3, p4, p5, p6, p7, p8, p10);
    } else if constexpr (is_braces_constructible_v<type, t, t, t, t, t, t, t, t, t>) {
        auto&& [p1, p2, p3, p4, p5, p6, p7, p8, p9] = obj;
        return std::make_tuple(p1, p2, p3, p4, p5, p6, p7, p8, p9);
    } else if constexpr (is_braces_constructible_v<type, t, t, t, t, t, t, t, t>) {
        auto&& [p1, p2, p3, p4, p5, p6, p7, p8] = obj;
        return std::make_tuple(p1, p2, p3, p4, p5, p6, p7, p8);
    } else if constexpr (is_braces_constructible_v<type, t, t, t, t, t, t, t>) {
        auto&& [p1, p2, p3, p4, p5, p6, p7] = obj;
        return std::make_tuple(p1, p2, p3, p4, p5, p6, p7);
    } else if constexpr (is_braces_constructible_v<type, t, t, t, t, t, t>) {
        auto&& [p1, p2, p3, p4, p5, p6] = obj;
        return std::make_tuple(p1, p2, p3, p4, p5, p6);
    } else if constexpr (is_braces_constructible_v<type, t, t, t, t, t>) {
        auto&& [p1, p2, p3, p4, p5] = obj;
        return std::make_tuple(p1, p2, p3, p4, p5);
    } else if constexpr (is_braces_constructible_v<type, t, t, t, t>) {
        auto&& [p1, p2, p3, p4] = obj;
        return std::make_tuple(p1, p2, p3, p4);
    } else if constexpr (is_braces_constructible_v<type, t, t, t>) {
        auto&& [p1, p2, p3] = obj;
        return std::make_tuple(p1, p2, p3);
    } else if constexpr (is_braces_constructible_v<type, t, t>) {
        auto&& [p1, p2] = obj;
        return std::make_tuple(p1, p2);
    } else if constexpr (is_braces_constructible_v<type, t>) {
        auto&& [p1] = obj;
        return std::make_tuple(p1);
    } else {
        WECS_ASSERT(false, "member number in POD > 10! Please reduce members or split them");
        return std::make_tuple();
    }
}

namespace internal {

template <typename Tuple, typename F, std::size_t... Indices>
void do_tuple_foreach(Tuple&& t, F&& f, std::index_sequence<Indices...>) {
    (f(std::get<Indices>(t)), ...);
}

}  // namespace internal

template <typename Tuple, typename F>
void tuple_foreach(Tuple&& t, F&& f) {
    internal::do_tuple_foreach(
        std::forward<Tuple>(t), 
        std::forward<F>(f),
        // {0, 1, 2, ..., tuple_size_v<Tuple> - 1}
        std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{});
}

}  // namespace wecs