/**
 * @file	test_sequence_finder.cpp
 * @brief	Tests if the sequence_finder find the sequences correctly.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "ext_deps/catch/catch.hpp"

#include "sequence_finder.hpp"

#include <string>

TEST_CASE("sequence_finder finds sequences in strings provided byte-by-byte", "[sequence_finder]")
{
    SECTION("Single sequence at the beginning")
    {
        jungles::sequence_finder f{"makapaka"};
        for (auto c : std::string{"makapak"})
        {
            REQUIRE(!f.found(c));
        }
        REQUIRE(f.found('a'));
    }
    SECTION("Single sequence in the middle")
    {
        jungles::sequence_finder f{"maka"};
        for (auto c : std::string{"dasdasdasmak"})
        {
            REQUIRE(!f.found(c));
        }
        REQUIRE(f.found('a'));
    }
    SECTION("Three sequences in a row")
    {
        jungles::sequence_finder f{"uga"};
        for (int i = 0; i < 3; ++i)
        {
            for (auto c : std::string{"ug"})
            {
                REQUIRE(!f.found(c));
            }
            REQUIRE(f.found('a'));
        }
    }
    SECTION("A sequence which contains string that is repeated in the sequence and this string is repeated 4 times so "
            "it's only found two times")
    {
        jungles::sequence_finder f{"ugauga"};
        for (int i = 0; i < 2; ++i)
        {
            for (auto c : std::string{"ugaug"})
            {
                REQUIRE(!f.found(c));
            }
            REQUIRE(f.found('a'));
        }
    }
}
