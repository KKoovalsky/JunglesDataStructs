/**
 * @file	main_catch.cpp
 * @brief	Runs Catch2 tests from this file
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 *
 * This file contains separated runner for Catch, because including Unity and Catch in the same file leads to
 * compilation errors.
 */

#define CATCH_CONFIG_RUNNER
#include "ext_deps/catch/catch.hpp"

int run_catch(int argc, char *argv[])
{
    return Catch::Session().run(argc, argv);
}
