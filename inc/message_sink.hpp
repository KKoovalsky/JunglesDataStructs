/**
 * @file	message_sink.hpp
 * @brief	Implements a message sink with constant size and which is able to figure out when a message arrives.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef MESSAGE_SINK_HPP
#define MESSAGE_SINK_HPP

#include "utils.hpp"
#include <algorithm>
#include <array>
#include <optional>
#include <tuple>
// #include <iterator>
// #include <iostream>

namespace jungles {

namespace detail {

template <typename T, std::size_t N> struct exceptional_sequence
{
    using SequenceIterator = utils::ArrayConstIterator<T, N>;

    const std::array<T, N> sequence;
    SequenceIterator sequence_it;
    bool search_continue;

    template <typename U>
    exceptional_sequence(U &&u) : sequence{std::forward<U>(u)}, sequence_it{std::begin(sequence)}, search_continue(true)
    {
    }

    void reset()
    {
        sequence_it = std::begin(sequence);
        search_continue = true;
    }
};

} // namespace detail

template <std::size_t InternalBufferSize,
          typename T,
          std::size_t TerminatorsNum,
          std::size_t... ExceptionalSequencesNum>
class message_sink
{
  private:
    using InternalBufferIt = typename std::array<T, InternalBufferSize>::iterator;

  public:
    template <typename S, typename... Ss>
    explicit constexpr message_sink(S &&message_terminators, Ss &&... exceptional_sequences)
        : m_terminators{std::forward<S>(message_terminators)},
          m_exceptional_sequences{std::forward<Ss>(exceptional_sequences)...}, m_buf_it{std::begin(m_buf)},
          m_search_continue{true}
    {
    }

    //! Use const_iterator to not allow the user to modify the internal buffer.
    using InternalBufferConstIt = typename std::array<T, InternalBufferSize>::const_iterator;

    std::optional<std::pair<InternalBufferConstIt, InternalBufferConstIt>> put_element_and_get_message(T elem)
    {
        auto is_terminator{[this](T elem) {
            return std::find(std::begin(m_terminators), std::end(m_terminators), elem) != std::end(m_terminators);
        }};
        auto is_overflow{[this]() { return m_buf_it == std::end(m_buf); }};

        if (is_terminator(elem))
            return reset_and_get_out_range();

        *m_buf_it++ = elem;
        if (is_overflow())
            return reset_and_get_out_range();

        return {};
    }

    // void print_all() const
    // {
    //     auto print{[](const auto &container) {
    //         std::copy(std::begin(container), std::end(container), std::ostream_iterator<T>{std::cout, ", "});
    //         std::cout << '\n';
    //     }};
    //     std::cout << "Terminators:\n";
    //     print(m_terminators);
    //     std::cout << "Exceptional sequences:\n";
    //     std::apply([&print](auto &&... p) { (print(p), ...); }, m_exceptional_sequences);
    // }

  private:
    const std::array<T, TerminatorsNum> m_terminators;
    std::tuple<detail::exceptional_sequence<T, ExceptionalSequencesNum>...> m_exceptional_sequences;
    std::array<T, InternalBufferSize> m_buf{};
    InternalBufferIt m_buf_it;
    bool m_search_continue;

    void reset_exceptional_sequence_search_algorithm()
    {
        std::apply([](auto &&... p) { (p.reset(), ...); }, m_exceptional_sequences);
    }

    std::optional<std::pair<InternalBufferConstIt, InternalBufferConstIt>> reset_and_get_out_range()
    {
        if (m_buf_it == std::begin(m_buf))
            return {};
        InternalBufferConstIt end{m_buf_it};
        m_buf_it = std::begin(m_buf);
        reset_exceptional_sequence_search_algorithm();
        m_search_continue = true;
        return std::pair{std::begin(m_buf), end};
    }

    bool is_exceptional_sequence_found(T elem)
    {
        if (!m_search_continue)
            return false;

        auto internal_lambda{[=](auto &s, auto elem) {
            if (!s.search_continue)
                return false;
            if (*s.sequence_it++ == elem)
            {
                if (s.sequence_it == std::end(s.sequence))
                    return true;
            }
            else
            {
                s.search_continue = false;
            }
            return false;
        }};
        auto lambda_for_apply{
            [elem, &internal_lambda](auto &&... sequences) { return (internal_lambda(sequences, elem) || ...); }};
        if (std::apply(lambda_for_apply, m_exceptional_sequences))
            return reset_and_get_out_range();

        m_search_continue = std::apply([](auto &&... sequences) { return (sequences.search_continue || ...); },
                                       m_exceptional_sequences);
    }
};

template <std::size_t InternalBufferSize, typename T, typename... Ts>
static constexpr auto make_message_sink(T &&message_terminators, Ts &&... exceptional_sequences)
{
    using Type = typename T::value_type;
    return message_sink<InternalBufferSize, Type, std::size(message_terminators), std::size(exceptional_sequences)...>{
        std::forward<T>(message_terminators), std::forward<Ts>(exceptional_sequences)...};
}

} // namespace jungles

#endif /* MESSAGE_SINK_HPP */
