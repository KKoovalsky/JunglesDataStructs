/**
 * @file	boundary_mapper.hpp
 * @brief	Maps boundaries to enum class values.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef BOUNDARY_MAPPER_HPP
#define BOUNDARY_MAPPER_HPP

#include <array>

namespace jungles {

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

/**
 * \brief Maps values to enum class.
 *
 * This class converts values from boundaries to enum class. The boundaries are provided as array of values. The
 * boundaries must be sorted in ascending order, e.g. 10, 20, 30. Taking this example, this class will split the
 * input values to such intervals:
 * I. (-inf, 10>
 * II. (10, 20>
 * III. (20, 30>
 * IV. (30, inf)
 * For each interval an enumerated value is assigned. The provided enum class must have the underlying values counting
 * started from 0 and incremented by one for consecutive values: e.g. enum class test_enum_class { A = 0, B, C, D };
 * For this example the intervals will be mapped like that:
 * I. (-inf, 10> - test_enum_class::A
 * II. (10, 20> - test_enum_class::B
 * III. (20, 30> - test_enum_class::C
 * IV. (30, inf) - test_enum_class::D
 * The enum class must have at least so many values as the size of the input array with boundaries + 1.
 *
 * It is preferred to construct the class using the factory function make_boundary_mapper() to use some template
 * auto-deduction.
 *
 * Usage example: map ADC values from leaf wetness sensor to levels of wetness.
 */
template <typename Enum,
          typename ValueType,
          std::size_t NumBoundaries,
          const std::array<ValueType, NumBoundaries> &Boundaries>
class boundary_mapper
{
    static_assert(std::is_enum_v<Enum>, "The first template argument must be an enum");
    static_assert(detail::is_ascending(std::begin(Boundaries), std::end(Boundaries)),
                  "The boundaries must be ascending");
    static_assert(NumBoundaries >= 1, "There must be at least two boundaries");

  public:
    constexpr Enum convert(ValueType val) const
    {
        unsigned i = 0;
        for (; i < m_num_boundaries; ++i)
            if (val <= m_boundaries[i])
                break;
        return static_cast<Enum>(i);
    }

  private:
    const std::array<ValueType, NumBoundaries> &m_boundaries = Boundaries;
    const std::size_t m_num_boundaries = NumBoundaries + 1;
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
