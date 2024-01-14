#pragma once 

#include <type_traits>
#include <utility>

namespace wecs {

namespace internal {

template <typename T>
constexpr bool is_ebco_to = std::is_empty_v<T> && !std::is_final_v<T>;

template <typename T, typename = void>
struct CompressPairElement final {
    using reference = T&;
    using const_reference = const T&;

    template <typename... Args>
    CompressPairElement(Args&&... args) noexcept(
        std::is_nothrow_constructible_v<T, Args...>)
        : value{std::forward<Args>(args)...} {}

    template <std::enable_if_t<std::is_default_constructible_v<T>, int> = 0>
    CompressPairElement() : value{} {}

    const_reference Get() const noexcept { return value; }

    reference Get() noexcept {
        return const_cast<reference>(std::as_const(*this).get());
    }

    T value;
};

template <typename T>
struct CompressPairElement<T, std::enable_if_t<is_ebco_to<T>>> final {
    using reference = T&;
    using const_reference = const T&;

    template <typename U>
    CompressPairElement(U&& t) noexcept {}

    reference Get() noexcept { return *this; }

    const_reference Get() const noexcept { return *this; }
};

template <typename First, typename Second>
class CompressPair final : private internal::CompressPairElement<First>,
                           private internal::CompressPairElement<Second> {
public:
    using first_base = internal::CompressPairElement<First>;
    using second_base = internal::CompressPairElement<Second>;

    using first_element_reference = First&;
    using first_element_const_reference = const First&;
    using second_element_reference = Second&;
    using second_element_const_reference = const Second&;

    CompressPair(const First& first, const Second& second)
        : first_base(first), second_base(second) {}

    CompressPair(First&& first, Second&& second)
        : first_base(std::move(first)), second_base(std::move(second)) {}

    first_element_reference First() noexcept {
        return static_cast<first_element_reference>(first_base::get());
    }

    second_element_reference Second() noexcept {
        return static_cast<second_element_reference>(second_base::get());
    }

    first_element_const_reference First() const noexcept {
        return const_cast<first_element_const_reference>(std::as_const(*this).first());
    }

    second_element_const_reference Second() const noexcept {
        return const_cast<second_element_const_reference>(std::as_const(*this).second());
    }
};

}  // namespace internal

}  // namespace wecs