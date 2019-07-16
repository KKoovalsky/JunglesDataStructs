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
    first,
    second,
    third
};

static constexpr auto boundaries{std::experimental::make_array<unsigned>(1, 2, 3, 4, 5)};

TEST_CASE("boundary_mapper template class unit tests", "[boundary_mapper]")
{
    // static constexpr jungles::boundary_mapper<test_enum_class, decltype(boundaries.itvalue_type, 5, boundaries> bm;
    static constexpr auto bm{jungles::make_boundary_mapper<test_enum_class, boundaries>()};
}
