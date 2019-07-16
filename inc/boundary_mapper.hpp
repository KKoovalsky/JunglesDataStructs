/**
 * @file	boundary_mapper.hpp
 * @brief	Maps boundaries to enum class values.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef BOUNDARY_MAPPER_HPP
#define BOUNDARY_MAPPER_HPP

namespace jungles {

#include <array>

namespace detail {

//! This is taken from std documentation, only 'constexpr' keyword has been added.
template <class ForwardIt, class Compare>
constexpr ForwardIt is_sorted_until_impl(ForwardIt first, ForwardIt last, Compare comp)
{
    if (first != last)
    {
        ForwardIt next = first;
        while (++next != last)
        {
            if (comp(*next, *first))
                return next;
            first = next;
        }
    }
    return last;
}

/**
 * This is taken from std documentation. It's 'std::is_sorted_until()' with a tweak to use std::less_equal and also
 * 'constexpr' keyword has been added.
 */
template <class ForwardIt> constexpr bool is_ascending(ForwardIt first, ForwardIt last)
{
    return is_sorted_until_impl(first, last, std::less_equal<>()) == last;
}

} // namespace detail

template <typename Enum,
          typename ValueType,
          std::size_t NumBoundaries,
          const std::array<ValueType, NumBoundaries> &Boundaries>
class boundary_mapper
{
    static_assert(std::is_enum_v<Enum>, "The first template argument must be an enum");
    static_assert(detail::is_ascending(std::begin(Boundaries), std::end(Boundaries)),
                  "The boundaries must be ascending");
    static_assert(NumBoundaries >= 2, "There must be at least two boundaries");

  public:
    constexpr Enum convert(ValueType val) const
    {
        return ValueType{};
    }

  private:
    const std::array<ValueType, NumBoundaries> &m_boundaries = Boundaries;
};

template <typename Enum, const auto &Boundaries> constexpr auto make_boundary_mapper()
{
    using T = std::decay_t<decltype(Boundaries)>;
    using V = typename T::value_type;
    constexpr auto N = std::size(Boundaries);
    return boundary_mapper<Enum, V, N, Boundaries>();
}

} // namespace jungles

#endif /* BOUNDARY_MAPPER_HPP */
