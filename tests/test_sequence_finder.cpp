/**
 * @file	test_sequence_finder.cpp
 * @brief	Tests if the sequence_finder find the sequences correctly.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "ext_deps/catch/catch.hpp"

#include "sequence_finder.hpp"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static std::string make_test_failure_message(bool shall_be_found, bool found, unsigned index);

TEST_CASE("sequence_finder finds sequences in strings provided byte-by-byte", "[sequence_finder]")
{
    using record = std::tuple<std::string, std::string_view, std::string, std::vector<unsigned>>;
    auto test_parameters = GENERATE(table<std::string, std::string_view, std::string, std::vector<unsigned>>(
        {record{"Single sequence at the beginning", "makapaka", "makapaka", {7}},
         record{"Single sequence in the middle", "maka", "dasdasdasmaka", {12}},
         record{"Three sequences in a row", "uga", "ugaugauga", {2, 5, 8}},
         record{"Repeated string within the sequence", "ugauga", "ugaugaugauga", {5, 11}}}));

    auto [test_description, sequence_to_find, input_string, indices_were_sequence_found] = test_parameters;
    SECTION(test_description)
    {
        for (auto index : indices_were_sequence_found)
        {
            if (index >= input_string.size())
            {
                FAIL("Test preparation failure: provided index: "
                     << index << " is out of bound for input string with size: " << input_string.size());
            }
        }

        jungles::sequence_finder sf{sequence_to_find};
        for (unsigned i = 0; i < input_string.size(); ++i)
        {
            const auto &indices = indices_were_sequence_found;

            auto shall_be_found{std::find(std::begin(indices), std::end(indices), i) != std::end(indices)};
            auto found = sf.found(input_string[i]);

            if ((shall_be_found && !found) || (!shall_be_found && found))
            {
                FAIL(make_test_failure_message(shall_be_found, found, i));
            }
        }
    }
}

static std::string make_test_failure_message(bool shall_be_found, bool found, unsigned index)
{
    std::ostringstream ss;
    ss << "At index " << index;
    ss << ", sequence shall" << (shall_be_found ? " " : " NOT ") << "be found, ";
    ss << "but" << (found ? " " : " NOT ") << "found";
    return ss.str();
};
