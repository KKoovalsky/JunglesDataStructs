/**
 * @file	test_boundary_mapper.cpp
 * @brief	Tests the boundary_mapper template class
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "ext_deps/catch/catch.hpp"

#include "boundary_mapper.hpp"

#include <experimental/array>

enum class test_enum_class
{
    first = 0,
    second,
    third,
    fourth,
    fifth,
    sixth,
};

static constexpr auto boundaries{std::experimental::make_array<unsigned>(1, 2, 3, 5, 7)};
static constexpr auto boundaries_lowest_number{std::experimental::make_array<unsigned>(50)};

TEST_CASE("boundary_mapper template class unit tests", "[boundary_mapper]")
{
    SECTION("Positive test for five boundaries")
    {
        static constexpr auto bm{jungles::make_boundary_mapper<test_enum_class, boundaries>()};
        REQUIRE(bm.convert(0) == test_enum_class::first);
        REQUIRE(bm.convert(1) == test_enum_class::first);
        REQUIRE(bm.convert(2) == test_enum_class::second);
        REQUIRE(bm.convert(3) == test_enum_class::third);
        REQUIRE(bm.convert(4) == test_enum_class::fourth);
        REQUIRE(bm.convert(5) == test_enum_class::fourth);
        REQUIRE(bm.convert(6) == test_enum_class::fifth);
        REQUIRE(bm.convert(7) == test_enum_class::fifth);
        REQUIRE(bm.convert(8) == test_enum_class::sixth);
        REQUIRE(bm.convert(9) == test_enum_class::sixth);
    }

    SECTION("Positive test for two boundaries (lowest possible number of boundaries)")
    {
        static constexpr auto bm{jungles::make_boundary_mapper<test_enum_class, boundaries_lowest_number>()};
        REQUIRE(bm.convert(49) == test_enum_class::first);
        REQUIRE(bm.convert(50) == test_enum_class::first);
        REQUIRE(bm.convert(51) == test_enum_class::second);
    }
}
