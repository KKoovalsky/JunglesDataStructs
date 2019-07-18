/**
 * @file	utils.hpp
 * @brief	Implements utilities for metaprogramming, helpers, generics...
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <array>
#include <cmath>
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

template <typename It, typename T> constexpr It find_nearest_boundary(It it, It end, T max_val, T val)
{
    if (it == end || val > max_val)
        return end;

    auto highest_boundary_it{std::prev(end)};
    auto lowest_boundary_it{it};
    auto highest_boundary = *highest_boundary_it;
    auto lowest_boundary = *lowest_boundary_it;
    if (val < lowest_boundary)
    {
        auto distance_to_lowest = lowest_boundary - val;
        auto distance_to_highest = max_val - highest_boundary + val;
        return distance_to_lowest > distance_to_highest ? highest_boundary_it : lowest_boundary_it;
    }
    else if (val > highest_boundary)
    {
        auto distance_to_lowest = max_val - val + lowest_boundary;
        auto distance_to_highest = val - highest_boundary;
        return distance_to_lowest > distance_to_highest ? highest_boundary_it : lowest_boundary_it;
    }

    auto min{max_val};
    auto res{it};
    for (; it != end; ++it)
    {
        if (auto v{std::abs(*it - val)}; v < min)
        {
            min = v;
            res = it;
        }
    }
    return res;
}

} // namespace utils

} // namespace jungles

#endif /* UTILS_HPP */
