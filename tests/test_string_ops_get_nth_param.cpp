/**
 * @file	test_string_ops_get_nth_param.cpp
 * @brief	Tests the functions which allows to obtain the nth param
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "string_ops.hpp"
#include "unity.h"
#include <string>

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
static void UNIT_TEST_1_get_first_parameter_only_one_provided();
static void UNIT_TEST_2_get_first_parameter_only_one_provided_with_delimiter_at_the_end();
static void UNIT_TEST_3_get_second_parameter_only_one_provided();
static void UNIT_TEST_4_get_second_parameter_two_provided();
static void UNIT_TEST_5_get_second_parameter_three_provided();
static void UNIT_TEST_6_get_third_parameter_two_provided_but_delimiter_at_the_end();
static void UNIT_TEST_7_positive_test_when_parameter_is_a_string();
static void UNIT_TEST_8_get_first_param_when_only_delimiter_provided();
static void UNIT_TEST_9_get_second_parameter_when_empty();
static void UNIT_TEST_10_get_second_parameter_when_first_empty();
static void UNIT_TEST_11_get_third_parameter_when_second_empty();
static void UNIT_TEST_12_get_third_parameter_when_first_and_second_empty();
static void UNIT_TEST_13_get_third_parameter_when_first();

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF PRIVATE FUNCTIONS AND VARIABLES
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// EXECUTION OF THE TESTS
// --------------------------------------------------------------------------------------------------------------------
void test_string_ops_get_nth_param()
{
    RUN_TEST(UNIT_TEST_1_get_first_parameter_only_one_provided);
    RUN_TEST(UNIT_TEST_2_get_first_parameter_only_one_provided_with_delimiter_at_the_end);
    RUN_TEST(UNIT_TEST_3_get_second_parameter_only_one_provided);
    RUN_TEST(UNIT_TEST_4_get_second_parameter_two_provided);
    RUN_TEST(UNIT_TEST_5_get_second_parameter_three_provided);
    RUN_TEST(UNIT_TEST_6_get_third_parameter_two_provided_but_delimiter_at_the_end);
    RUN_TEST(UNIT_TEST_7_positive_test_when_parameter_is_a_string);
}

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
static void UNIT_TEST_1_get_first_parameter_only_one_provided()
{
    TEST_ASSERT_EQUAL_STRING("makapaka", jungles::get_nth_param("makapaka", ',', 0).c_str());
}

static void UNIT_TEST_2_get_first_parameter_only_one_provided_with_delimiter_at_the_end()
{
    TEST_ASSERT_EQUAL_STRING("makapaka", jungles::get_nth_param("makapaka,", ',', 0).c_str());
}

static void UNIT_TEST_3_get_second_parameter_only_one_provided()
{
    TEST_ASSERT_TRUE(jungles::get_nth_param("makapaka", ',', 1).empty());
}

static void UNIT_TEST_4_get_second_parameter_two_provided()
{
    TEST_ASSERT_EQUAL_STRING("szakalaka", jungles::get_nth_param("makapaka,szakalaka", ',', 1).c_str());
}

static void UNIT_TEST_5_get_second_parameter_three_provided()
{
    TEST_ASSERT_EQUAL_STRING("szakalaka", jungles::get_nth_param("makapaka,szakalaka,mastablasta", ',', 1).c_str());
}

static void UNIT_TEST_6_get_third_parameter_two_provided_but_delimiter_at_the_end()
{
    TEST_ASSERT_TRUE(jungles::get_nth_param("makapaka,szakalaka,", ',', 2).empty());
}

static void UNIT_TEST_7_positive_test_when_parameter_is_a_string()
{
    std::string input{"some,input,yolo"};
    TEST_ASSERT_EQUAL_STRING("input", jungles::get_nth_param(input, ',', 1).c_str());
}
// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF PRIVATE FUNCTIONS
// --------------------------------------------------------------------------------------------------------------------

