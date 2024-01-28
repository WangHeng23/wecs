#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "wecs/entity/resource.hpp"

using namespace wecs;

struct Res {
    int value;
};

TEST_CASE("resource") {
    internal::resource_cache<Res>::instance().emplace(Res{123});

    SECTION("un-mutable case") {
        resource<Res> res;
        auto& r = res.get();
        static_assert(std::is_const_v<std::remove_reference_t<decltype(r)>>);
        REQUIRE(r.value == 123);
    }

    SECTION("mutable case") {
        resource<mut<Res>> res;
        auto& r = res.get();
        static_assert(!std::is_const_v<std::remove_reference_t<decltype(r)>>);
        REQUIRE(r.value == 123);
        r.value = 789;

        resource<Res> res2;
        REQUIRE(res2.get().value == 789);
    }
}