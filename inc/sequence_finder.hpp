/**
 * @file	sequence_finder.hpp
 * @brief	Implements a class which is responsible for finding a sequence by single characters.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <string_view>

namespace jungles {

/**
 * \brief Allows to find sequences provided by character.
 *
 * This is useful when you get characters byte-by-byte from some external source (e.g. USART interrupt) and want to
 * search for some occurences on-the-fly.
 */
class sequence_finder
{
  public:
    //! Constructs the sequence finder. When the sequence_to_find is empty then the behaviour is undefined.
    constexpr explicit sequence_finder(std::string_view sequence_to_find) noexcept
        : m_sequence_to_find{sequence_to_find}, m_index{0}
    {
    }

    constexpr bool found(char c) noexcept
    {
        if (c == m_sequence_to_find[m_index])
        {
            ++m_index;
            if (m_index == m_sequence_to_find.size())
            {
                m_index = 0;
                return true;
            }
        }
        else
        {
            m_index = 0;
        }
        return false;
    }

  private:
    std::string_view m_sequence_to_find;
    unsigned m_index;
};

} // namespace jungles
