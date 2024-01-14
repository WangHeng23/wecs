#include "wecs/core/utility.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <string>

using namespace wecs;

TEST_CASE("functionalities") {
    constexpr auto zero_is_power_of_two = is_power_of_two(0u);

    REQUIRE_FALSE(zero_is_power_of_two);
    REQUIRE(is_power_of_two(1u));
    REQUIRE(is_power_of_two(2u));
    REQUIRE_FALSE(is_power_of_two(3u));
    REQUIRE(is_power_of_two(4u));
    REQUIRE_FALSE(is_power_of_two(7u));
    REQUIRE(is_power_of_two(4096u));
    REQUIRE_FALSE(is_power_of_two(10000u));
    REQUIRE(is_power_of_two(4294967296u));
    REQUIRE_FALSE(is_power_of_two(4294967297u));
}

double d = 123.45;

struct POD {
    int a;
    float b;
    double c;
    std::string s;

    static POD create() {
        return {1, 2.0f, d, "wecs"};
    }
};

TEST_CASE("extract pod members") {
    POD pod = POD::create();
    static_assert(wecs::is_braces_constructible_v<POD, int, float, double, std::string>);
    static_assert(!wecs::is_braces_constructible_v<POD, float, float, double, std::string>);
    static_assert(!wecs::is_braces_constructible_v<POD, float, double, std::string>);

    auto members = wecs::extract_pod_members(pod);
    REQUIRE(std::tuple_size_v<decltype(members)> == 4);
}