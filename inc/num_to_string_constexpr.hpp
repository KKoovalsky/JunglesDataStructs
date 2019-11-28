/**
 * @file	num_to_string_constexpr.hpp
 * @brief	Utility to convert number to string at compile time
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef NUM_TO_STRING_CONSTEXPR_HPP
#define NUM_TO_STRING_CONSTEXPR_HPP

namespace jungles {

namespace utils {

namespace detail {

template <unsigned... digits> struct to_chars
{
    static constexpr char value[] = {('0' + digits)..., 0};
};

template <unsigned rem, unsigned... digits> struct explode : explode<rem / 10, rem % 10, digits...>
{
};

template <unsigned... digits> struct explode<0, digits...> : to_chars<digits...>
{
};

} // namespace detail

/**
 * \brief Converts unsigned integer to string literal at compile time.
 *
 * Use it e.g. like that:
 * constexpr char *num_str{jungles::utils::num_to_string<321312>::value};
 */
template <unsigned num> struct num_to_string : detail::explode<num>
{
};

} // namespace utils

} // namespace jungles

#endif /* NUM_TO_STRING_CONSTEXPR_HPP */
