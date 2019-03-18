/**
 * @file	string_ops.c
 * @brief	Defines string operations
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include <string_view>

namespace jungles {

std::string_view get_nth_param(std::string_view s, char delimiter, unsigned num_param)
{
    unsigned pos = 0;
    auto pos_end = s.find(delimiter);
    for (; pos_end != std::string_view::npos && num_param; --num_param)
    {
        pos = pos_end + 1;
        pos_end = s.find(delimiter, pos);
    }

    if (num_param)
        return {};

    if (pos_end == std::string_view::npos)
        pos_end = s.size();
    return std::string_view(std::next(s.begin(), pos), pos_end - pos);
}

} // namespace jungles
