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

/**
 * \brief Finds the nearest value, where the values are provided as sorted range <it, end), for specific value val and
 *        taking into account the maximum value max_val. A
 *
 * This means that when the range contains values e.g. 100, 200, 300 and the maximum value is 600 then:
 * * for val = 120 the result is 100
 * * for val = 155 the result is 200
 * * for val = 290 the result is 200
 * * for val = 0 the result is 100
 * * for val = 501 the result is 100
 * * for val = 499 the result is 300
 *
 * \see The test_find_nearest.cpp which contains tests and explains in more details the idea behind that.
 */
template <typename It, typename T> constexpr It find_nearest(It it, It end, T max_val, T val)
{
    if (it == end || val > max_val)
        return end;

    auto highest_val_it{std::prev(end)};
    auto lowest_val_it{it};
    auto highest_val = *highest_val_it;
    auto lowest_val = *lowest_val_it;
    if (val < lowest_val)
    {
        auto distance_to_lowest = lowest_val - val;
        auto distance_to_highest = max_val - highest_val + val;
        return distance_to_lowest > distance_to_highest ? highest_val_it : lowest_val_it;
    }
    else if (val > highest_val)
    {
        auto distance_to_lowest = max_val - val + lowest_val;
        auto distance_to_highest = val - highest_val;
        return distance_to_lowest > distance_to_highest ? highest_val_it : lowest_val_it;
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
