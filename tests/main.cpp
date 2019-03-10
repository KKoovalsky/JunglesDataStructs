/**
 * @file	main.cpp
 * @brief	Definition of main() for unit tests.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "unity.h"

extern void test_ibytestream_ostringstream();
extern void test_member_isr_enable();
extern void test_event_handler();

int main()
{
    UNITY_BEGIN();

    test_ibytestream_ostringstream();
    test_member_isr_enable();
    test_event_handler();

    return UNITY_END();
}
