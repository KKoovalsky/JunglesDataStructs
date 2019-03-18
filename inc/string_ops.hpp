/**
 * @file	string_ops.hpp
 * @brief	Declares functions which operate on strings
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef STRING_OPS_HPP
#define STRING_OPS_HPP

#include <algorithm>
#include <iostream>
#include <iterator>

namespace jungles {

template <typename InStringType, typename ElementType>
std::string get_nth_param(const InStringType &s, ElementType delimiter, unsigned num_param)
{
    auto end{std::end(s)};
    auto it1{std::begin(s)};
    auto it2{std::find(it1, end, delimiter)};
    for (; it2 != end && num_param; --num_param)
    {
        it1 = std::next(it2, 1);
        it2 = std::find(it1, end, delimiter);
    }

    if ((num_param > 0) || (std::distance(it1, it2) == 1 && (*it1) == '\0'))
        return {};
    else
        return std::string(it1, it2);
}

} // namespace jungles

#endif /* STRING_OPS_HPP */

