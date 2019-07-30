/**
 * @file	binary_stream.hpp
 * @brief	Implements a binary stream template class.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef BINARY_STREAM_HPP
#define BINARY_STREAM_HPP

#include "utils.hpp"
#include <cinttypes>

namespace jungles {

namespace detail {

//! This is dangerous and can only be used internally unless you know what you are doing.
template <typename T, typename It> void write_and_advance(It &it, T val)
{
    auto to_ptr = &(*it);
    auto to_byte_ptr = reinterpret_cast<uint8_t *>(to_ptr);

    auto from_ptr = &val;
    auto from_byte_ptr = reinterpret_cast<uint8_t *>(from_ptr);

    std::copy(from_byte_ptr, from_byte_ptr + sizeof(T), to_byte_ptr);
    using IterableType = typename std::iterator_traits<It>::value_type;
    std::advance(it, sizeof(T) / sizeof(IterableType));
}

} // namespace detail

template <std::size_t InternalBufSize> class binary_stream
{
  private:
    using Byte = uint8_t;
    using ByteArrayType = std::array<Byte, InternalBufSize>;

  public:
    using BinaryStreamConstIterator = typename ByteArrayType::const_iterator;

    /**
     * \brief Tries to write all of the parameters to the stream.
     * \param[in] params Variables of trivial types which will be streamed.
     * \returns true when all of the params were put to the stream, false if none of the params have been put.
     * \note If there is no place for all parameters to be but then no parameter is put to the stream.
     */
    template <typename... TrivialTypes> bool write(TrivialTypes... params)
    {
        constexpr unsigned sizeof_params = (sizeof(params) + ... + 0);
        static_assert(jungles::utils::all_trivial<TrivialTypes...>::value, "The parameters must be trivial types");
        if (space_left() < sizeof_params)
            return false;

        (jungles::detail::write_and_advance(m_it, params), ...);
        return true;
    }

    //! Clears the stream and removes all the data from it.
    void clear()
    {
        m_it = std::begin(m_buf);
    }

    //! Returns begin iterator to the stream. Never invalidated.
    BinaryStreamConstIterator cbegin() const
    {
        return std::cbegin(m_buf);
    }

    //! Returns end iterator to the stream. Gets invalidated after call to write() or clear().
    BinaryStreamConstIterator cend() const
    {
        return static_cast<BinaryStreamConstIterator>(m_it);
    }

  private:
    using BinaryStreamIterator = typename ByteArrayType::iterator;

    ByteArrayType m_buf;
    BinaryStreamIterator m_it{std::begin(m_buf)};

    unsigned space_left() const
    {
        return std::distance(static_cast<BinaryStreamConstIterator>(m_it), std::end(m_buf));
    }
};

} // namespace jungles

#endif /* BINARY_STREAM_HPP */
