/**
 * @file	test_ibytestream_ostringstream.cpp
 * @brief	Tests ibytestream_ostringstream class.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "ibytestream_ostringstream.hpp"
#include "unity.h"

using namespace jungles;

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
static void UNIT_TEST_push_string_by_byte_and_pop_string();
static void UNIT_TEST_push_multiple_strings_by_byte_and_pop_strings();
static void UNIT_TEST_push_string_make_empty_and_push_again_and_make_empty_again();
static void UNIT_TEST_push_multiple_empty_lines();
static void UNIT_TEST_push_string_finished_with_crlf();
static void UNIT_TEST_push_string_finished_with_lf();
static void UNIT_TEST_push_string_finished_with_cr();
static void UNIT_TEST_push_string_finished_with_null_terminator();
static void UNIT_TEST_push_exceptional_character();
static void UNIT_TEST_push_exceptional_character_and_newline_after_that();
static void UNIT_TEST_push_exceptional_character_in_the_middle_of_the_string();
static void UNIT_TEST_push_string_with_multiple_different_exceptional_characters();
static void UNIT_TEST_push_string_finished_with_exceptional_character_then_next_string();

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF PRIVATE FUNCTIONS AND VARIABLES
// --------------------------------------------------------------------------------------------------------------------
template <size_t N, size_t M>
static void helper_push_string_by_byte(ibytestream_ostringstream<N, M> &b, const std::string &s);
template <size_t N, size_t M>
static void helper_push_string_terminated_with_newline_by_byte(ibytestream_ostringstream<N, M> &b,
                                                               const std::string &s);

// --------------------------------------------------------------------------------------------------------------------
// EXECUTION OF THE TESTS
// --------------------------------------------------------------------------------------------------------------------
void test_ibytestream_ostringstream()
{
    RUN_TEST(UNIT_TEST_push_string_by_byte_and_pop_string);
    RUN_TEST(UNIT_TEST_push_multiple_strings_by_byte_and_pop_strings);
    RUN_TEST(UNIT_TEST_push_string_make_empty_and_push_again_and_make_empty_again);
    RUN_TEST(UNIT_TEST_push_multiple_empty_lines);
    RUN_TEST(UNIT_TEST_push_string_finished_with_crlf);
    RUN_TEST(UNIT_TEST_push_string_finished_with_lf);
    RUN_TEST(UNIT_TEST_push_string_finished_with_cr);
    RUN_TEST(UNIT_TEST_push_string_finished_with_null_terminator);
    RUN_TEST(UNIT_TEST_push_exceptional_character);
    RUN_TEST(UNIT_TEST_push_exceptional_character_and_newline_after_that);
    RUN_TEST(UNIT_TEST_push_exceptional_character_in_the_middle_of_the_string);
    RUN_TEST(UNIT_TEST_push_string_with_multiple_different_exceptional_characters);
    RUN_TEST(UNIT_TEST_push_string_finished_with_exceptional_character_then_next_string);
}

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
static void UNIT_TEST_push_string_by_byte_and_pop_string()
{
    ibytestream_ostringstream<64, 16> b;

    std::string expected("BUENAVISTA");

    helper_push_string_terminated_with_newline_by_byte(b, expected);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), b.pop_string().c_str());
}

static void UNIT_TEST_push_multiple_strings_by_byte_and_pop_strings()
{
    ibytestream_ostringstream<64, 16> b;

    std::string expected1("BUENAVISTA"), expected2("COSTORITSO"), expected3("LAVUENTE");

    helper_push_string_terminated_with_newline_by_byte(b, expected1);
    helper_push_string_terminated_with_newline_by_byte(b, expected2);
    helper_push_string_terminated_with_newline_by_byte(b, expected3);

    TEST_ASSERT_EQUAL_STRING(expected1.c_str(), b.pop_string().c_str());
    TEST_ASSERT_EQUAL_STRING(expected2.c_str(), b.pop_string().c_str());
    TEST_ASSERT_EQUAL_STRING(expected3.c_str(), b.pop_string().c_str());
}

static void UNIT_TEST_push_string_make_empty_and_push_again_and_make_empty_again()
{
    ibytestream_ostringstream<64, 16> b;

    std::string expected1("SOLDATI");
    helper_push_string_terminated_with_newline_by_byte(b, expected1);
    TEST_ASSERT_EQUAL_STRING(expected1.c_str(), b.pop_string().c_str());

    std::string expected2("MARIANNOPASTI");
    helper_push_string_terminated_with_newline_by_byte(b, expected2);
    TEST_ASSERT_EQUAL_STRING(expected2.c_str(), b.pop_string().c_str());
}

static void UNIT_TEST_push_multiple_empty_lines()
{
    ibytestream_ostringstream<64, 16> b;
    TEST_ASSERT(b.push_byte_and_is_string_end('\n') == false);
    TEST_ASSERT(b.push_byte_and_is_string_end('\n') == false);
    TEST_ASSERT(b.push_byte_and_is_string_end('\n') == false);
    TEST_ASSERT(b.push_byte_and_is_string_end('\n') == false);
    TEST_ASSERT(b.is_empty() == true);
}

static void UNIT_TEST_push_string_finished_with_crlf()
{
    ibytestream_ostringstream<64, 16> b;

    std::string expected("COMPRENDTE");

    helper_push_string_by_byte(b, expected);
    TEST_ASSERT(b.push_byte_and_is_string_end('\r') == true);
    TEST_ASSERT(b.push_byte_and_is_string_end('\n') == false);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), b.pop_string().c_str());
}

static void UNIT_TEST_push_string_finished_with_lf()
{
    ibytestream_ostringstream<64, 16> b;

    std::string expected("CASUSBELI");

    helper_push_string_by_byte(b, expected);
    TEST_ASSERT(b.push_byte_and_is_string_end('\n') == true);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), b.pop_string().c_str());
}

static void UNIT_TEST_push_string_finished_with_cr()
{
    ibytestream_ostringstream<64, 16> b;

    std::string expected("ANNUALOPARSTA");

    helper_push_string_by_byte(b, expected);
    TEST_ASSERT(b.push_byte_and_is_string_end('\r') == true);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), b.pop_string().c_str());
}

static void UNIT_TEST_push_string_finished_with_null_terminator()
{
    ibytestream_ostringstream<64, 16> b;

    std::string expected("GUANOAPESBANDEROS");

    helper_push_string_by_byte(b, expected);
    TEST_ASSERT(b.push_byte_and_is_string_end('\0') == true);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), b.pop_string().c_str());
}

static void UNIT_TEST_push_exceptional_character()
{
    ibytestream_ostringstream<64, 16> b(">?!");
    TEST_ASSERT(b.push_byte_and_is_string_end('>') == true);
    TEST_ASSERT_EQUAL_STRING(">", b.pop_string().c_str());
    TEST_ASSERT(b.push_byte_and_is_string_end('?') == true);
    TEST_ASSERT_EQUAL_STRING("?", b.pop_string().c_str());
    TEST_ASSERT(b.is_empty() == true);
}

static void UNIT_TEST_push_exceptional_character_and_newline_after_that()
{
    ibytestream_ostringstream<64, 16> b(">?!");

    TEST_ASSERT(b.push_byte_and_is_string_end('>') == true);
    TEST_ASSERT(b.push_byte_and_is_string_end('\r') == false);
    TEST_ASSERT(b.push_byte_and_is_string_end('\n') == false);
    TEST_ASSERT_EQUAL_STRING(">", b.pop_string().c_str());
    TEST_ASSERT(b.is_empty() == true);
}

static void UNIT_TEST_push_exceptional_character_in_the_middle_of_the_string()
{
    ibytestream_ostringstream<64, 16> b("?>!");

    std::string expected("UMBARA>TUGUIA");

    helper_push_string_terminated_with_newline_by_byte(b, expected);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), b.pop_string().c_str());
}

static void UNIT_TEST_push_string_with_multiple_different_exceptional_characters()
{
    ibytestream_ostringstream<64, 16> b("?>!");

    std::string expected("COME>TO?PAPA!>MYLOVE??BAYBE");
    helper_push_string_terminated_with_newline_by_byte(b, expected);

    TEST_ASSERT_EQUAL_STRING(expected.c_str(), b.pop_string().c_str());
}

static void UNIT_TEST_push_string_finished_with_exceptional_character_then_next_string()
{
    ibytestream_ostringstream<64, 16> b("?>!");

    std::string expected1("MASTERZZZZ>");
    helper_push_string_terminated_with_newline_by_byte(b, expected1);
    TEST_ASSERT_EQUAL_STRING(expected1.c_str(), b.pop_string().c_str());

    std::string expected2("GOWERNORRRR");
    helper_push_string_terminated_with_newline_by_byte(b, expected2);
    TEST_ASSERT_EQUAL_STRING(expected2.c_str(), b.pop_string().c_str());
}

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF PRIVATE FUNCTIONS
// --------------------------------------------------------------------------------------------------------------------
template <size_t N, size_t M>
static void helper_push_string_by_byte(ibytestream_ostringstream<N, M> &b, const std::string &s)
{
    for (unsigned i = 0; i < s.length(); ++i)
        TEST_ASSERT(b.push_byte_and_is_string_end(s[i]) == false);
}

template <size_t N, size_t M>
static void helper_push_string_terminated_with_newline_by_byte(ibytestream_ostringstream<N, M> &b, const std::string &s)
{
    for (unsigned i = 0; i < s.length(); ++i)
        TEST_ASSERT(b.push_byte_and_is_string_end(s[i]) == false);
    TEST_ASSERT(b.push_byte_and_is_string_end('\n') == true);
}
