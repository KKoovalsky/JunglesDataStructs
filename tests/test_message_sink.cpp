/**
 * @file	test_message_sink.cpp
 * @brief	Tests the message_sink template.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "message_sink.hpp"
#include "unity.h"

using namespace jungles;

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
static void UNIT_TEST_1_positive_create_message_sink();
static void UNIT_TEST_2_only_one_terminator();
static void UNIT_TEST_3_multiple_terminators();
static void UNIT_TEST_4_no_message_returned_when_two_terminators_in_a_row();
static void UNIT_TEST_5_whole_buffer_returned_on_overflow();
static void UNIT_TEST_6_single_sequence_found();
static void UNIT_TEST_7_single_sequence_in_the_middle_of_message_ignored();
static void UNIT_TEST_8_multiple_sequences_found();
static void UNIT_TEST_9_few_terminated_messages_then_few_empty_then_terminated_then_exceptional_sequences();
static void UNIT_TEST_10_few_terminated_messages_with_crlf();
static void UNIT_TEST_11_few_terminated_then_one_exceptional_sequence_then_terminated_message();
static void UNIT_TEST_12_few_exceptional_sequences_then_few_terminated_messages();
static void UNIT_TEST_13_exceptional_almost_found_but_last_element_terminator();
static void UNIT_TEST_14_exceptional_almost_found_but_last_element_didnt_match();
static void UNIT_TEST_15_two_sequences_which_have_the_same_prefix();
static void UNIT_TEST_16_five_sequences_which_have_the_same_prefix();

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF PRIVATE FUNCTIONS AND VARIABLES
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// DEFINITIONS OF PUBLIC HELPER LAMBDAS
// --------------------------------------------------------------------------------------------------------------------
static auto helper_put_string_to_sink_and_terminate_and_get_result{[](auto &sink, const auto &string, auto terminator) {
    for (auto c : string)
        TEST_ASSERT_FALSE(sink.put_element_and_get_message(c).has_value());
    auto r = sink.put_element_and_get_message(terminator);
    TEST_ASSERT_TRUE(r.has_value());
    auto [beg_it, end_it] = *r;
    return std::string(beg_it, end_it);
}};

static auto helper_put_string_to_sink_and_terminate{[](auto &sink, const auto &string, auto terminator) {
    TEST_ASSERT_EQUAL_STRING(string.c_str(),
                             helper_put_string_to_sink_and_terminate_and_get_result(sink, string, terminator).c_str());
}};

static auto helper_put_string_to_sink_and_check_terminated_on_last_put{[](auto &sink, const auto &string) {
    auto previous_to_last_it = std::prev(std::end(string));
    std::string string_without_last_char(std::begin(string), previous_to_last_it);
    auto last_string_elem = *previous_to_last_it;
    TEST_ASSERT_EQUAL_STRING(
        string.c_str(),
        helper_put_string_to_sink_and_terminate_and_get_result(sink, string_without_last_char, last_string_elem)
            .c_str());
}};

// --------------------------------------------------------------------------------------------------------------------
// EXECUTION OF THE TESTS
// --------------------------------------------------------------------------------------------------------------------
void test_message_sink()
{
    RUN_TEST(UNIT_TEST_1_positive_create_message_sink);
    RUN_TEST(UNIT_TEST_2_only_one_terminator);
    RUN_TEST(UNIT_TEST_3_multiple_terminators);
    RUN_TEST(UNIT_TEST_4_no_message_returned_when_two_terminators_in_a_row);
    RUN_TEST(UNIT_TEST_5_whole_buffer_returned_on_overflow);
    RUN_TEST(UNIT_TEST_6_single_sequence_found);
    RUN_TEST(UNIT_TEST_7_single_sequence_in_the_middle_of_message_ignored);
    RUN_TEST(UNIT_TEST_8_multiple_sequences_found);
    RUN_TEST(UNIT_TEST_9_few_terminated_messages_then_few_empty_then_terminated_then_exceptional_sequences);
    RUN_TEST(UNIT_TEST_10_few_terminated_messages_with_crlf);
    RUN_TEST(UNIT_TEST_11_few_terminated_then_one_exceptional_sequence_then_terminated_message);
    RUN_TEST(UNIT_TEST_12_few_exceptional_sequences_then_few_terminated_messages);
    RUN_TEST(UNIT_TEST_13_exceptional_almost_found_but_last_element_terminator);
    RUN_TEST(UNIT_TEST_14_exceptional_almost_found_but_last_element_didnt_match);
    RUN_TEST(UNIT_TEST_15_two_sequences_which_have_the_same_prefix);
    RUN_TEST(UNIT_TEST_16_five_sequences_which_have_the_same_prefix);
}

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
static void UNIT_TEST_1_positive_create_message_sink()
{
    auto char_sink{make_message_sink<256>(
        std::array{'\r', '\n', '\0'}, std::array{'>'}, std::array{'M', 'A', 'K', 'A', 'P', 'A', 'K', 'A'})};
    auto int_sink{make_message_sink<256>(std::array{0, 1, 2, 3}, std::array{4, 5}, std::array{7, 8, 9, 10})};
}

static void UNIT_TEST_2_only_one_terminator()
{
    auto sink{make_message_sink<256>(std::array{'\r'})};
    std::string s1{"MAKAPAKA"}, s2{"UGABUGA"};

    helper_put_string_to_sink_and_terminate(sink, s1, '\r');
    helper_put_string_to_sink_and_terminate(sink, s2, '\r');
}

static void UNIT_TEST_3_multiple_terminators()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n', '\0'})};
    std::string s1{"EOULO"}, s2{"HEYBEYB"}, s3{"ALOHABIATCH"};

    helper_put_string_to_sink_and_terminate(sink, s1, '\n');
    helper_put_string_to_sink_and_terminate(sink, s2, '\r');
    helper_put_string_to_sink_and_terminate(sink, s3, '\r');
}

static void UNIT_TEST_4_no_message_returned_when_two_terminators_in_a_row()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n', '\0'})};
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\r').has_value());
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\n').has_value());
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\0').has_value());
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\r').has_value());
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\0').has_value());
}

static void UNIT_TEST_5_whole_buffer_returned_on_overflow()
{
    constexpr std::size_t size{64};
    auto sink{make_message_sink<size>(std::array{'\n'})};
    std::string s(size, 'a');
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, s);
}

static void UNIT_TEST_6_single_sequence_found()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n', '\0'}, utils::to_array("MAKA"))};
    std::string s{"MAKA"};
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, s);
}

static void UNIT_TEST_7_single_sequence_in_the_middle_of_message_ignored()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n', '\0'}, utils::to_array("MAKA"))};
    std::string s{"AMAKAdsadsa"};
    for (auto c : s)
        TEST_ASSERT_FALSE(sink.put_element_and_get_message(c).has_value());
}

static void UNIT_TEST_8_multiple_sequences_found()
{
    auto sink{make_message_sink<256>(
        std::array{'\r', '\n', '\0'}, utils::to_array("MAKA"), utils::to_array("PAKA"), utils::to_array("SZAKA"))};
    std::string s1{"MAKA"}, s2{"PAKA"}, s3{"SZAKA"};
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, s1);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, s2);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, s3);
}

static void UNIT_TEST_9_few_terminated_messages_then_few_empty_then_terminated_then_exceptional_sequences()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n'}, utils::to_array(">"), utils::to_array("HEY"))};
    std::string msg1{"HELLO"}, msg2{"WORLD"};
    std::string seq1{">"}, seq2{"HEY"};

    helper_put_string_to_sink_and_terminate(sink, msg1, '\n');
    helper_put_string_to_sink_and_terminate(sink, msg2, '\r');

    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\r').has_value());
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\r').has_value());
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\n').has_value());

    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, seq1);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, seq1);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, seq2);
}

static void UNIT_TEST_10_few_terminated_messages_with_crlf()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n'})};
    std::string msg1{"HAJLABAJLA"}, msg2{"PROECO"}, msg3{"PROSECO"};

    helper_put_string_to_sink_and_terminate(sink, msg1, '\r');
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\r').has_value());
    helper_put_string_to_sink_and_terminate(sink, msg2, '\r');
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\r').has_value());
    helper_put_string_to_sink_and_terminate(sink, msg3, '\r');
    TEST_ASSERT_FALSE(sink.put_element_and_get_message('\r').has_value());
}

static void UNIT_TEST_11_few_terminated_then_one_exceptional_sequence_then_terminated_message()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n'}, utils::to_array("EXCEPTIONAL"))};
    std::string msg1{"BIATCH"}, msg2{"BEACH"};
    std::string seq1{"EXCEPTIONAL"};

    helper_put_string_to_sink_and_terminate(sink, msg1, '\n');
    helper_put_string_to_sink_and_terminate(sink, msg2, '\r');

    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, seq1);

    helper_put_string_to_sink_and_terminate(sink, msg2, '\r');
}

static void UNIT_TEST_12_few_exceptional_sequences_then_few_terminated_messages()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n'}, utils::to_array("EXCEPTIONAL"), utils::to_array("BEYBE"))};
    std::string msg1{"ERECTUS"}, msg2{"SAPIENS"};
    std::string seq1{"EXCEPTIONAL"}, seq2{"BEYBE"};

    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, seq1);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, seq1);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, seq2);

    helper_put_string_to_sink_and_terminate(sink, msg1, '\n');
    helper_put_string_to_sink_and_terminate(sink, msg2, '\r');
    helper_put_string_to_sink_and_terminate(sink, msg2, '\r');
    helper_put_string_to_sink_and_terminate(sink, msg2, '\r');
}

static void UNIT_TEST_13_exceptional_almost_found_but_last_element_terminator()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n'}, utils::to_array("EXCEPTIONAL"))};
    std::string msg1{"EXCEPTIONA"}, msg2{"SOME_RANDOM_MSG"};
    helper_put_string_to_sink_and_terminate(sink, msg1, '\r');
    helper_put_string_to_sink_and_terminate(sink, msg2, '\r');
}

static void UNIT_TEST_14_exceptional_almost_found_but_last_element_didnt_match()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n'}, utils::to_array("EXCEPTIONAL"))};
    std::string msg{"EXCEPTIONABOUT"};
    helper_put_string_to_sink_and_terminate(sink, msg, '\r');
}

static void UNIT_TEST_15_two_sequences_which_have_the_same_prefix()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n'}, utils::to_array("BRUMBUR"), utils::to_array("BRUMBUS"))};
    std::string msg1{"BRUMBUR"}, msg2{"BRUMBUS"};
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, msg1);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, msg2);
}

static void UNIT_TEST_16_five_sequences_which_have_the_same_prefix()
{
    auto sink{make_message_sink<256>(std::array{'\r', '\n'},
                                     utils::to_array("BROWAR"),
                                     utils::to_array("BROMBERG"),
                                     utils::to_array("BROFIST"),
                                     utils::to_array("BROTHER"),
                                     utils::to_array("BROBLAST"))};

    std::string msg1{"BROWAR"}, msg2{"BROMBERG"}, msg3{"BROFIST"}, msg4{"BROTHER"}, msg5{"BROBLAST"};
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, msg1);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, msg2);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, msg3);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, msg4);
    helper_put_string_to_sink_and_check_terminated_on_last_put(sink, msg5);
}

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF PRIVATE FUNCTIONS
// --------------------------------------------------------------------------------------------------------------------

