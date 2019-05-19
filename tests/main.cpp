/**
 * @file	main.cpp
 * @brief	Definition of main() for unit tests.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "unity.h"
#include <iostream>
#include <string>

extern void test_ibytestream_ostringstream();
extern void test_member_isr_enable();
extern void test_event_handler();
extern void test_string_ops_get_nth_param();
extern void test_message_sink();

int main()
{
    auto print_ruler{[](unsigned length, char ruler_elem) {
        return [=]() { std::cout << std::string(length, ruler_elem) << "\n"; };
    }};
    auto ruler{print_ruler(120, '=')};

    UNITY_BEGIN();

    test_ibytestream_ostringstream();
    ruler();
    test_member_isr_enable();
    ruler();
    test_event_handler();
    ruler();
    test_string_ops_get_nth_param();
    ruler();
    test_message_sink();

    return UNITY_END();
}
