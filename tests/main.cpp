/**
 * @file	main.cpp
 * @brief	Definition of main() for unit tests.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "unity.h"
#include <iostream>
#include <string>

#define CATCH_CONFIG_RUNNER
#include "catch/catch.hpp"

extern void test_ibytestream_ostringstream();
extern void test_member_isr_enable();
extern void test_event_handler();
extern void test_string_ops_get_nth_param();
extern void test_message_sink();

int main(int argc, char *argv[])
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

    auto res = UNITY_END();

    std::cout << "\r\nUnity tests finished with status: " << res << std::endl;
    std::cout << "\r\nRunning Catch tests\r\n" << std::endl;

    return Catch::Session().run(argc, argv);
}

