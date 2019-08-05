/**
 * @file	test_binary_stream.cpp
 * @brief	Tests the binary_stream template class.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "ext_deps/catch/catch.hpp"

#include "binary_stream.hpp"

#include <cinttypes>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

TEST_CASE("binary_stream template class unit tests", "[binary_stream]")
{
    SECTION("Succesfully writes single variable when there is sufficient space")
    {
        jungles::binary_stream<10> bs;
        REQUIRE(bs.write(static_cast<uint32_t>(0x00112233)));
        const uint8_t result_little_endian[] = {0x33, 0x22, 0x11, 0x00};
        REQUIRE(std::equal(bs.cbegin(), bs.cend(), std::begin(result_little_endian)));
    }

    SECTION("Succesfully writes multiple variables when there is sufficient space")
    {
        jungles::binary_stream<20> bs;
        REQUIRE(bs.write(static_cast<uint32_t>(0x44332211), static_cast<char>(0x6F), static_cast<short>(0xFFEE)));
        const uint8_t result_little_endian[] = {0x11, 0x22, 0x33, 0x44, 0x6F, 0xEE, 0xFF};
        REQUIRE(std::equal(bs.cbegin(), bs.cend(), std::begin(result_little_endian)));
    }

    SECTION("Doesn't write single variable when there is no sufficient space")
    {
        jungles::binary_stream<8> bs;
        REQUIRE(bs.write(static_cast<uint32_t>(0xAABBCCDD), static_cast<uint16_t>(0xEEFF)));
        REQUIRE_FALSE(bs.write(static_cast<uint32_t>(0x00112233)));
        const uint8_t result_little_endian[] = {0xDD, 0xCC, 0xBB, 0xAA, 0xFF, 0xEE};
        REQUIRE(std::equal(bs.cbegin(), bs.cend(), std::begin(result_little_endian)));
    }

    SECTION("Doesn't write multiple variables when there is no sufficient space")
    {
        jungles::binary_stream<12> bs;
        REQUIRE(bs.write(static_cast<uint32_t>(0xAABBCCDD), static_cast<uint16_t>(0xEEFF)));
        REQUIRE_FALSE(bs.write(static_cast<uint32_t>(0x00112233), static_cast<uint32_t>(0x44556677)));
        const uint8_t result_little_endian[] = {0xDD, 0xCC, 0xBB, 0xAA, 0xFF, 0xEE};
        REQUIRE(std::equal(bs.cbegin(), bs.cend(), std::begin(result_little_endian)));
    }

    SECTION("Doesn't allow to write after writing to the edge of the stream")
    {
        SECTION("When writing single variables")
        {
            jungles::binary_stream<8> bs;
            REQUIRE(bs.write(static_cast<uint32_t>(99)));
            REQUIRE(bs.write(static_cast<uint32_t>(99)));
            REQUIRE_FALSE(bs.write(static_cast<uint32_t>(99)));
            REQUIRE(std::distance(bs.cbegin(), bs.cend()) == 8);
        }
        SECTION("When writing multiple variables")
        {
            jungles::binary_stream<8> bs;
            REQUIRE(bs.write(static_cast<uint32_t>(99), static_cast<uint32_t>(99)));
            REQUIRE_FALSE(bs.write(static_cast<uint32_t>(99)));
            REQUIRE(std::distance(bs.cbegin(), bs.cend()) == 8);
        }
    }

    SECTION("After clearing begin is same like end")
    {
        jungles::binary_stream<14> bs;
        REQUIRE(bs.write(311));
        REQUIRE(bs.cbegin() != bs.cend());
        bs.clear();
        REQUIRE(bs.cbegin() == bs.cend());
    }
}
