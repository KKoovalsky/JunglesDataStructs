/**
 * @file	test_member_isr_enable.c
 * @brief	Tests metaprogramming which enables a member function to be ISR handler
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "member_isr_enable.hpp"
#include "unity.h"

class some_class
{
  public:
    some_class(unsigned u, char c) : m_u{u}, m_c{c}
    {
    }

  public:
    unsigned isr_handler(unsigned a, unsigned b)
    {
        return a + b;
    }
    unsigned m_u;
    char m_c;
};

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
static void UNIT_TEST_1_isr_handler_with_ret_val_and_multiple_params();

// --------------------------------------------------------------------------------------------------------------------
// DECLARATION OF PRIVATE FUNCTIONS AND VARIABLES
// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------
// EXECUTION OF THE TESTS
// --------------------------------------------------------------------------------------------------------------------
void test_member_isr_enable()
{
    RUN_TEST(UNIT_TEST_1_isr_handler_with_ret_val_and_multiple_params);
}

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF THE TEST CASES
// --------------------------------------------------------------------------------------------------------------------
unsigned (*fp)(unsigned, unsigned);
static void UNIT_TEST_1_isr_handler_with_ret_val_and_multiple_params()
{
    jungles::make_and_enable_member_as_isr_handler<fp, some_class, &some_class::isr_handler>(static_cast<unsigned>(5),
                                                                                             'a');
    auto r = (*fp)(5, 6);
    TEST_ASSERT_EQUAL_UINT(11, r);
}

// --------------------------------------------------------------------------------------------------------------------
// DEFINITION OF PRIVATE FUNCTIONS
// --------------------------------------------------------------------------------------------------------------------

