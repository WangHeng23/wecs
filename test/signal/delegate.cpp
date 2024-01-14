#include "wecs/signal/delegate.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace wecs;

double Multiply2(float x) {
    return x * 2.0f;
}

float Sum(float x[]) {
    return x[0] + x[1] + x[2];
}

bool OrderedParams(std::string, float, double) {
    return true;
}

struct Person final {
    const std::string& GetName() const { return name; }

    std::string name;
    float height;
};

TEST_CASE("delegate") {
    SECTION("simple global function delagate") {
        delegate<double(float)> d;
        d.connect<&Multiply2>();
        REQUIRE(d(2.0f) == 4.0f);
    }

    SECTION("delegate with payload") {
        delegate<double()> d;
        float value = 123.0f;
        d.connect<&Multiply2>(value);
        REQUIRE(d() == 246.0f);

        float value_[] = {1.0f, 2.0f, 3.6f};
        d.connect<&Sum>(value_);
        REQUIRE(d() == 6.6f);
    }
    
    SECTION("delegate with ordered arguments") {
        delegate<bool(float, std::string, double)> d;
        d.connect<&OrderedParams>(std::index_sequence<1, 0, 2>{});
        REQUIRE(d(1.0f, "wecs", 3.0));
    }

    SECTION("work with class") {
        Person person;
        person.name = "foo";
        person.height = 122.0f;

        delegate<std::string(void)> d1;
        d1.connect<&Person::name>(person);
        REQUIRE(d1() == "foo");

        delegate<std::string(Person&)> d2;
        d2.connect<&Person::name>();
        REQUIRE(d2(person) == "foo");

        delegate<float(Person&)> d2_;
        d2_.connect<&Person::height>();
        REQUIRE(d2_(person) == 122.0f);

        delegate<const std::string&(Person&)> d3;
        d3.connect<&Person::GetName>();
        REQUIRE(d3(person) == "foo");
    }
}