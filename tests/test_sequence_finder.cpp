/**
 * @file	test_sequence_finder.cpp
 * @brief	Tests if the sequence_finder find the sequences correctly.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "ext_deps/catch/catch.hpp"

#include "sequence_finder.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class sequence_finder_matcher : public Catch::MatcherBase<jungles::sequence_finder>
{
  public:
    sequence_finder_matcher(std::function<bool(jungles::sequence_finder)> matcher) : m_matcher{matcher}
    {
    }

    bool match(const jungles::sequence_finder &sf) const override
    {
        return m_matcher(sf);
    }

    virtual std::string describe() const override
    {
        std::ostringstream ss;
        ss << "finds sequence in ";
        return ss.str();
    }

  private:
    std::function<bool(jungles::sequence_finder)> m_matcher;
};

struct InString
{
    InString(std::string s) : m_s{s}
    {
    }

    sequence_finder_matcher FindsSequenceAtIndex(unsigned index)
    {
        return FindsSequenceAtIndices({index});
    }

    sequence_finder_matcher FindsSequenceAtIndices(std::vector<unsigned> indices)
    {
        return sequence_finder_matcher{[input_string = this->m_s, indices](jungles::sequence_finder sf) {
            // auto make_output_message{[](bool shall_be_found, bool found, unsigned index) {
            //     std::ostringstream ss;
            //     ss << "At index" << index;
            //     ss << ", sequence shall" << (shall_be_found ? " " : "not ") << "be found, ";
            //     ss << "but" << (found ? " " : "not ") << "found";
            //     return ss.str();
            // }};

            for (auto index : indices)
                assert(index < input_string.size() && "Index is out-of-bound for the input string");
            for (unsigned i = 0; i < input_string.size(); ++i)
            {
                auto found = sf.found(input_string[i]);
                auto shall_be_found{std::find(std::begin(indices), std::end(indices), i) != std::end(indices)};
                if ((shall_be_found && !found) || (!shall_be_found && found))
                    return false;
            }
            return true;
        }};
    }

    std::string m_s;
};

TEST_CASE("sequence_finder finds sequences in strings provided byte-by-byte", "[sequence_finder]")
{
    SECTION("Single sequence at the beginning")
    {
        REQUIRE_THAT(jungles::sequence_finder{"makapaka"}, InString("makapaka").FindsSequenceAtIndex(7));
    }
    SECTION("Single sequence in the middle")
    {
        REQUIRE_THAT(jungles::sequence_finder{"maka"}, InString("dasdasdasmaka").FindsSequenceAtIndex(12));
    }
    SECTION("Three sequences in a row")
    {
        REQUIRE_THAT(jungles::sequence_finder{"uga"}, InString("ugaugauga").FindsSequenceAtIndices({2, 5, 8}));
    }
    SECTION("A sequence which contains string that is repeated in the sequence and this string is repeated 4 times so "
            "it's only found two times")
    {
        REQUIRE_THAT(jungles::sequence_finder{"ugauga"}, InString("ugaugaugauga").FindsSequenceAtIndices({5, 11}));
    }
}
