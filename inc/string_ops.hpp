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

/**
 * \brief Get nth parameter from the input string taking into account the delimiter.
 *
 * E.g. get_nth_param("maka,paka,szaka", ',', 1) returns "paka"
 */
std::string_view get_nth_param(std::string_view s, char delimiter, unsigned num_param);

} // namespace jungles

#endif /* STRING_OPS_HPP */

