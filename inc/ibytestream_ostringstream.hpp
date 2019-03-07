/**
 * @file	ibytestream_ostringstream.hpp
 * @brief	Defines a stream which accepts bytes and returns strings
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef IBYTE_STREAM_OSTRING_STREAM_HPP
#define IBYTE_STREAM_OSTRING_STREAM_HPP

#include "cyclic_buf.hpp"
#include <array>
#include <string>
#include <string_view>

namespace jungles {

namespace detail {
//! Calculates the length of span between two indexes in the cyclic buffer.
static inline unsigned calc_len_in_circular_buffer(unsigned beg_idx, unsigned end_idx, unsigned cyclic_buf_size);

} // namespace detail

/**
 * \brief Push single bytes to it and pop strings. Useful when receiving messages using interrupts.
 *
 * This implementation makes use of that it is known where the beginning and the end of a line is placed in the
 * internal buffer. To use this knowledge we need to store the ends of lines somewhere. To avoid allocations in
 * interrupt handlers another cyclic buffer is used to store those ends. Its size is specified at the time of
 * template instantiation and it determines how many lines can fit into the buffer.
 *
 * When the internal buffer overflows or maximum number of strings is reached then the behaviour is undefined.
 *
 * The typical usage is that: push single bytes to it (e.g. on an interrupt on byte received), pop a command when
 * a whole command has been received.
 *
 * \note
 * I have been thinking a lot about that how to handle those overflows and the best solution is to ignore them as they
 * occur only when used improperly (so never). The internal buffer size and the maximum number of strings in the buffer
 * shall be dependent on the system. KK
 *
 */
template <size_t InternalBufSize, size_t MaxNumStringsInBuf> class ibytestream_ostringstream
{
  public:
    /**
     * Takes string which contains exceptional characters which should be treated as a whole strings even when a
     * string terminator hasn't arrived. E.g. when you want to treat single '>' as a full string then pass it
     * in the argument. It is allowed to treat various characters in this manner.
     */
    explicit ibytestream_ostringstream(std::string_view exceptional_chars = std::string_view{""},
                                       std::string_view string_terminators = std::string_view{"\0\r\n", 3});

    /**
     * Push a byte and check whether the byte terminates the string.
     * By default the string terminators are: CR, LF and '\0'
     */
    bool push_byte_and_is_string_end(char c);

    //! Pop a whole string. When there is no string this returns an empty string.
    std::string pop_string();

    bool is_empty();

  private:
    //! This is a helper object which holds the indexes of the commands' ends. @todo Allocator pvPortMalloc
    cyclic_buf<unsigned int, MaxNumStringsInBuf> m_end_indexes_cb;

    //! The cyclic buffer where the commands are held.
    cyclic_buf<char, InternalBufSize> m_cb;

    //! Contains the exceptional characters which should be treated as whole strings.
    const std::string_view m_exceptional_chars;

    //! Contains the characters which determine the string end.
    const std::string_view m_string_terminators;

    //! Last head index in the immediate buffer.
    unsigned m_last_end_idx{0};
};

template <size_t InternalBufSize, size_t MaxNumStringsInBuf>
ibytestream_ostringstream<InternalBufSize, MaxNumStringsInBuf>::ibytestream_ostringstream(
    std::string_view exceptional_chars, std::string_view string_terminators)
    : m_exceptional_chars(exceptional_chars), m_string_terminators(string_terminators)
{
}

template <size_t InternalBufSize, size_t MaxNumStringsInBuf>
bool ibytestream_ostringstream<InternalBufSize, MaxNumStringsInBuf>::push_byte_and_is_string_end(char c)
{
    if (m_string_terminators.find(c) != std::string::npos)
    {
        // When received a command of length 0 then do nothing.
        if (m_last_end_idx == m_cb.head)
            return false;
        else
        {
            m_end_indexes_cb.push_elem(m_cb.head);
            m_last_end_idx = m_cb.head;
            return true;
        }
    }

    // After receiving the exceptional character:
    if (m_exceptional_chars.find(c) != std::string::npos)
    {
        // Exceptional characters work only when they are received alone.
        if (m_last_end_idx == m_cb.head)
        {
            m_cb.push_elem(c);
            m_end_indexes_cb.push_elem(m_cb.head);
            m_last_end_idx = m_cb.head;
            return true;
        }
    }

    // Push any other character to the buffer.
    m_cb.push_elem(c);

    return false;
}

template <size_t InternalBufSize, size_t MaxNumStringsInBuf>
std::string ibytestream_ostringstream<InternalBufSize, MaxNumStringsInBuf>::pop_string()
{
    // Firstly check whether there are lines in the buffer. If not then return immediately.
    if (is_empty())
        return "";

    auto beg = m_cb.tail;
    auto end = m_end_indexes_cb.pop_elem();
    unsigned int len = detail::calc_len_in_circular_buffer(beg, end, InternalBufSize);

    // Get the string to be returned.
    std::string s(len, '\0');
    m_cb.pop_nelems(&s.front(), len);

    return std::move(s);
}

template <size_t InternalBufSize, size_t MaxNumStringsInBuf>
bool ibytestream_ostringstream<InternalBufSize, MaxNumStringsInBuf>::is_empty()
{
    return m_end_indexes_cb.is_empty();
}

namespace detail {
static inline unsigned calc_len_in_circular_buffer(unsigned beg_idx, unsigned end_idx, unsigned cyclic_buf_size)
{
    unsigned int len;
    // Check whether there will be a swing.
    if (beg_idx > end_idx)
        len = cyclic_buf_size - beg_idx + end_idx;
    else
        len = end_idx - beg_idx;
    return len;
}
} // namespace detail

} // namespace jungles

#endif /* IBYTE_STREAM_OSTRING_STREAM_HPP */
