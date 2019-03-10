/**
 * @file	test_event_handler.cpp
 * @brief	Tests the event handler.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "event_handler.hpp"
#include "unity.h"

using namespace jungles;

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
static void UNIT_TEST_1_raise_no_handler_registered();
static void UNIT_TEST_2_raise_single_handler_registered();
static void UNIT_TEST_3_raise_multiple_handlers_registered();
static void GIVEN_multiple_handlers_reged_WHEN_raised_AND_WHEN_one_policy_deregister_THEN_deregistered();

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF PRIVATE FUNCTIONS AND VARIABLES
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// EXECUTION OF THE TESTS
// --------------------------------------------------------------------------------------------------------------------
void test_event_handler()
{
    RUN_TEST(UNIT_TEST_1_raise_no_handler_registered);
    RUN_TEST(UNIT_TEST_2_raise_single_handler_registered);
    RUN_TEST(UNIT_TEST_3_raise_multiple_handlers_registered);
    RUN_TEST(GIVEN_multiple_handlers_reged_WHEN_raised_AND_WHEN_one_policy_deregister_THEN_deregistered);
}

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
static void UNIT_TEST_1_raise_no_handler_registered()
{
    event_handler eh;
    eh.raise();
}

static void UNIT_TEST_2_raise_single_handler_registered()
{
    event_handler eh;
    unsigned res = 0;
    eh.register_handler([&res]() {
        res = 1;
        return event_handler::policy::keep;
    });
    eh.raise();
    TEST_ASSERT_EQUAL_UINT(1, res);
}

static void UNIT_TEST_3_raise_multiple_handlers_registered()
{
    event_handler eh;
    unsigned res = 0;
    const unsigned count = 10;
    for (unsigned i = 0; i < count; ++i)
    {
        eh.register_handler([&res, i]() {
            res |= (1 << i);
            return event_handler::policy::keep;
        });
    }
    eh.raise();
    TEST_ASSERT_EQUAL_UINT((1 << count) - 1, res);
}

static void GIVEN_multiple_handlers_reged_WHEN_raised_AND_WHEN_one_policy_deregister_THEN_deregistered()
{
    event_handler eh;
    unsigned res = 0;
    eh.register_handler([&res]() {
        res |= 1;
        return event_handler::policy::keep;
    });
    eh.register_handler([&res]() {
        res |= 2;
        return event_handler::policy::deregister;
    });
    eh.register_handler([&res]() {
        res |= 4;
        return event_handler::policy::keep;
    });
    eh.raise();
    res = 0;
    eh.raise();
    TEST_ASSERT_EQUAL_UINT(1 | 4, res);
}

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF PRIVATE FUNCTIONS
// --------------------------------------------------------------------------------------------------------------------

