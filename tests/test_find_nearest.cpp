/**
 * @file	test_find_nearest.cpp
 * @brief	Tests the boundary_mapper template class
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "ext_deps/catch/catch.hpp"

#include "utils.hpp"

#include <experimental/array>
#include <vector>

TEST_CASE("find_nearest() function unit tests", "[find_nearest]")
{
    SECTION("Returns end when begin and end are the same")
    {
        std::vector<int> v;
        auto it{std::begin(v)};
        REQUIRE(jungles::utils::find_nearest(it, it, 10000, 42) == it);
    }

    SECTION("Returns end when input value is bigger than max value")
    {
        static constexpr auto a{std::experimental::make_array(3213, 2332, 3213, 3213121)};
        REQUIRE(jungles::utils::find_nearest(std::begin(a), std::end(a), 300, 330) == std::end(a));
    }

    SECTION("Finds in the middle of the set with rounding up and down")
    {
        static constexpr auto a{std::experimental::make_array(500, 600, 1100, 2000)};
        // Round up
        REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 3300, 1000) == 1100);
        // Round down
        REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 3300, 700) == 600);
    }

    SECTION("Finds on the edge of intervals with rounding up and down")
    {
        static constexpr auto a{std::experimental::make_array(100, 200, 300)};
        REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 1000, 149) == 100);
        REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 1000, 151) == 200);
    }

    SECTION("Finds boundary on the edge and takes into account the max value")
    {
        SECTION("Finds when the boundary is before the max value, so between the last boundary and max value")
        {
            static constexpr auto a{std::experimental::make_array(500, 600, 1100, 2000)};
            REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 3300, 0) == 500);
            REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 3300, 3300) == 500);
            REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 3300, 2901) == 500);
            REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 3300, 2899) == 2000);
        }

        SECTION("Finds when the boundary is after the max value, so between zero and the first boundary")
        {
            static constexpr auto a{std::experimental::make_array(500, 600, 1100, 2000)};
            REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 2200, 0) == 2000);
            REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 2200, 2200) == 2000);
            REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 2200, 149) == 2000);
            REQUIRE(*jungles::utils::find_nearest(std::begin(a), std::end(a), 2200, 151) == 500);
        }
    }
}
