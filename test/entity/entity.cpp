#include "wecs/wecs.hpp"
#include "wecs/config/config.hpp"
#include "wecs/entity/entity.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace wecs;

#define Entity(x) static_cast<entity>(x)

TEST_CASE("utilities") {
    REQUIRE(internal::popcount(0b1111) == 4);
    REQUIRE(internal::popcount(0b11111111111111) == 14);
    REQUIRE(internal::popcount(0x0) == 0);
    REQUIRE(internal::popcount(0b001001010) == 3);

    REQUIRE(internal::entity_to_integral<entity>(Entity(3)) == 3);
    REQUIRE(internal::entity_id<entity>(Entity(3)) == 3);
    REQUIRE(internal::entity_id<entity>(Entity(10086)) == 10086);

    auto m_Entity = internal::construct_entity<entity>(3, 1);
    REQUIRE(internal::entity_version<entity>(m_Entity) == 3);
    REQUIRE(internal::entity_next_version<entity>(m_Entity) == 4);
    REQUIRE(internal::entity_id<entity>(m_Entity) == 1);

    auto m_NextEntity = internal::entity_inc_version(m_Entity);
    REQUIRE(internal::entity_version<entity>(m_NextEntity) == 4);
    REQUIRE(internal::entity_id<entity>(m_NextEntity) == 1);

    auto m_Entity2 = internal::construct_entity<entity>(0, 4);
    auto m_Entity3 = internal::construct_entity<entity>(4, 4);
    REQUIRE(internal::combine_entity(m_NextEntity, m_Entity2) == m_Entity3);

    auto m_Entity4 = internal::construct_entity<entity>(0xFFF, 0xFFFFF);
    auto m_Entity5 = internal::construct_entity<entity>(10086, 0xFFFFF);
    auto m_NullEntity = null_entity;
    REQUIRE((m_NullEntity == null_entity));
    REQUIRE_FALSE((m_NullEntity != null_entity));
    REQUIRE(m_NullEntity == m_Entity4);
    REQUIRE(m_NullEntity == m_Entity5);
    REQUIRE(m_NullEntity != m_Entity3);
    REQUIRE_FALSE(m_NullEntity != m_Entity4);

    //-----------------------------
    auto m_Entity6 = internal::construct_entity<entity>(4095, 0);
    auto m_Entity7 = internal::construct_entity<entity>(4096, 0);
    auto m_Entity8 = internal::construct_entity<entity>(4097, 0);
    REQUIRE(internal::entity_version<entity>(m_Entity6) == 4095);
    REQUIRE(internal::entity_version<entity>(m_Entity7) == 0);
    REQUIRE(internal::entity_version<entity>(m_Entity8) == 1);
}