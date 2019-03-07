/**
 * @file	main.cpp
 * @brief	Definition of main() for unit tests.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "unity.h"

extern void test_ibytestream_ostringstream();

int main()
{
    UNITY_BEGIN();

    test_ibytestream_ostringstream();

    return UNITY_END();
}
