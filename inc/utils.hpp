/**
 * @file	utils.hpp
 * @brief	Implements utilities for metaprogramming, helpers, generics...
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <array>
#include <tuple>

namespace jungles {

namespace utils {

template <typename T, std::size_t N> using ArrayConstIterator = typename std::array<T, N>::const_iterator;
template <typename T, std::size_t N> using ArrayIterator = typename std::array<T, N>::iterator;
template <typename T, std::size_t... Ns> using ArrayConstIterators = typename std::array<T, Ns...>::const_iterator;

template <bool...> struct bool_pack;

template <bool... b> using all_true = std::is_same<bool_pack<true, b...>, bool_pack<b..., true>>;

template <typename> struct is_std_array : std::false_type
{
};

template <typename T, std::size_t N> struct is_std_array<std::array<T, N>> : std::true_type
{
};

template <std::size_t N, std::size_t... Is>
constexpr std::array<char, N - 1> to_array(const char (&a)[N], std::index_sequence<Is...>)
{
    return {{a[Is]...}};
}

template <std::size_t N> constexpr std::array<char, N - 1> to_array(const char (&a)[N])
{
    return to_array(a, std::make_index_sequence<N - 1>());
}

template <typename E> constexpr auto to_u_type(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

} // namespace utils

} // namespace jungles

#endif /* UTILS_HPP */
