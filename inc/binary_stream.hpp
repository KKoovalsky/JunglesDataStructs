/**
 * @file	binary_stream.hpp
 * @brief	Implements a binary stream template class.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#ifndef BINARY_STREAM_HPP
#define BINARY_STREAM_HPP

#include <cinttypes>

namespace jungles {

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
    template <typename... TrivialTypes> bool write(TrivialTypes &&... params);

    //! Clears the stream and removes all the data from it.
    void clear();

    //! Returns begin iterator to the stream. Never invalidated.
    BinaryStreamConstIterator cbegin() const;

    //! Returns end iterator to the stream. Gets invalidated after call to write() or clear().
    BinaryStreamConstIterator cend() const;

  private:
    using BinaryStreamIterator = typename ByteArrayType::iterator;

    ByteArrayType m_buf;
    BinaryStreamIterator m_it;
};

} // namespace jungles

#endif /* BINARY_STREAM_HPP */
