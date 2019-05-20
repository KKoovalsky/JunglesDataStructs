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

namespace jungles {

namespace detail {

//! Used internally within the message_sink class.
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

    bool is_found(T elem)
    {
        // If earlier one element didn't match the sequence then return false until the conditions, listed in the
        // description of message_sink::put_element_and_get_message(), are met. Then this flag is reset to true.
        if (!search_continue)
            return false;

        if (*sequence_it++ == elem)
        {
            // This functions returns true only when the sequence is aligned at the beginning of the incoming
            // message.
            if (sequence_it == std::end(sequence))
                return true;
        }
        else
        {
            // The search for the sequence is aborted when the received element is not equal to the currently
            // indexed element in the sequence.
            search_continue = false;
        }

        return false;
    }
};

} // namespace detail

// TODO: WRITE WTF ABOUT THIS CLASS AND README.MD AS WELL
template <std::size_t InternalBufferSize,
          typename T,
          std::size_t TerminatorsNum,
          std::size_t... ExceptionalSequencesNum>
class message_sink
{
  private:
    using InternalBufferIt = typename std::array<T, InternalBufferSize>::iterator;

  public:
    /**
     * \brief Constructor which is able to move- or copy-assign the message terminators and exceptional sequences.
     *
     * \param[in] message_terminators   The terminators of the incoming messages (e.g. most frequently for <char>:
     *                                  CR, LF, \0) passed as an std::array of T.
     * \param[in] exceptional_sequence  The sequences which will be treated as a messages which don't need terminator
     *                                  on the end of the message. Passed as variable number std::array's of T.
     *
     * \note There is a helper function called make_message_sink() which allows to use partial type deduction.
     */
    template <typename S, typename... Ss>
    explicit constexpr message_sink(S &&message_terminators, Ss &&... exceptional_sequences)
        : m_terminators{std::forward<S>(message_terminators)},
          m_exceptional_sequences{std::forward<Ss>(exceptional_sequences)...}, m_buf_it{std::begin(m_buf)},
          m_exceptional_sequences_search_continue{true}
    {
    }

    //! Use const_iterator to not allow the user to modify the internal buffer.
    using InternalBufferConstIt = typename std::array<T, InternalBufferSize>::const_iterator;

    /**
     * \brief Puts an element to the message sink and returns a range when a whole message is received or buffer
     * overflows. A message is received when:
     * - the element elem is a terminator,
     * - the messages matches any of the specified exceptional sequence.
     *
     * When there is no more space in the buffer the message is also returned.
     *
     * The range is a pair of const_iterator's to the beginning and end of the received message.
     *
     * \returns std::pair of const_iterator's to the beginning and end of the received message when the conditions are
     *          met or default-initialized std::optional otherwise.
     */
    std::optional<std::pair<InternalBufferConstIt, InternalBufferConstIt>> put_element_and_get_message(T elem)
    {
        auto is_terminator{[this](T elem) {
            return std::find(std::begin(m_terminators), std::end(m_terminators), elem) != std::end(m_terminators);
        }};
        auto is_buffer_overflowed{[this]() { return m_buf_it == std::end(m_buf); }};

        if (is_terminator(elem))
            return reset_and_get_out_range();

        *m_buf_it++ = elem;
        if (is_buffer_overflowed() || is_exceptional_sequence_found(elem))
            return reset_and_get_out_range();

        return {};
    }

  private:
    const std::array<T, TerminatorsNum> m_terminators;
    std::tuple<detail::exceptional_sequence<T, ExceptionalSequencesNum>...> m_exceptional_sequences;

    //! The internal buffer which is a sink.
    std::array<T, InternalBufferSize> m_buf{};

    //! Iterator used to put elements to the internal buffer.
    InternalBufferIt m_buf_it;

    //! A flag which indicates whether there is possibility that the message being received is an exceptional sequence.
    bool m_exceptional_sequences_search_continue;

    void reset_exceptional_sequence_search_algorithm()
    {
        std::apply([](auto &&... p) { (p.reset(), ...); }, m_exceptional_sequences);
    }

    std::optional<std::pair<InternalBufferConstIt, InternalBufferConstIt>> reset_and_get_out_range()
    {
        // Guard for situations when multiple terminators are received in a row.
        if (m_buf_it == std::begin(m_buf))
            return {};

        // Reset all the internals to the state which is the same like after construction.
        InternalBufferConstIt end{m_buf_it};
        m_buf_it = std::begin(m_buf);
        reset_exceptional_sequence_search_algorithm();
        m_exceptional_sequences_search_continue = true;

        return std::pair{std::begin(m_buf), end};
    }

    bool is_exceptional_sequence_found(T elem)
    {
        if (!m_exceptional_sequences_search_continue)
            return false;

        // Call exceptional_sequence::is_found() for each element in the tuple to check whether any of the sequences
        // has been found.
        auto is_any_sequence_found_impl{[elem](auto &&... sequences) { return (sequences.is_found(elem) || ...); }};
        auto is_any_sequence_found{
            [&, this]() { return std::apply(is_any_sequence_found_impl, m_exceptional_sequences); }};
        if (is_any_sequence_found())
            return true;

        // If no exceptional sequence was found at the beginning of the message, which is being received, then abort
        // searching for a sequence, because it is assumed that the exceptional sequences can be only found at the
        // beginning of the incoming message (exceptional sequences are left-aligned only).
        // When no sequence is found then the m_exceptional_sequences_search_continue flag is reset when a terminator is
        // received or the buffer is overflowed.
        auto is_any_search_continue_flag_true_impl{
            [](auto &&... sequences) { return (sequences.search_continue || ...); }};
        auto is_any_search_continue_flag_true{
            [&, this] { return std::apply(is_any_search_continue_flag_true_impl, m_exceptional_sequences); }};
        m_exceptional_sequences_search_continue = is_any_search_continue_flag_true();

        return false;
    }
};

//! Helper function used to create a message_sink with a partial type deduction.
template <std::size_t InternalBufferSize, typename T, typename... Ts>
static constexpr auto make_message_sink(T &&message_terminators, Ts &&... exceptional_sequences)
{
    using Type = typename T::value_type;
    return message_sink<InternalBufferSize, Type, std::size(message_terminators), std::size(exceptional_sequences)...>{
        std::forward<T>(message_terminators), std::forward<Ts>(exceptional_sequences)...};
}

} // namespace jungles

#endif /* MESSAGE_SINK_HPP */
