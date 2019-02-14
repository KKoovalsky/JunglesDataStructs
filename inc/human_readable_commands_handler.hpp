/**
 * @file	human_readable_commands_handler.hpp
 * @brief	A handler which handles commands which are human readable.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "cyclic_buf.hpp"
#include <string>
#include <vector>

//! The default size of the buffer where the indexes with commands' ends are held.
constexpr size_t ends_indexes_cb_def_size = 16;

/**
 * \brief Stores human readable commands.
 *
 * This implementation assumes that the commands are popped in a short time after pushing them. Otherwise the
 * behaviour is undefined.
 *
 * The typical usage is that: push single bytes to it (e.g. on an interrupt on byte received), pop a command when
 * a whole command has been received.
 *
 * \todo	Make the cyclic buffers resizeable.
 * \todo	Keep track of the commands on overflow, because when an overflow in one of the buffers occurs the data
 *			between the buffers is inconsistent: the indexes to the end of the buffers may not properly point to
 *			the real ends of commands.
 * \todo	Implement pushing multiple elements.
 */
template <size_t N> class human_readable_commands_handler
{
  private:
	//! This is a helper object which holds the indexes of the commands' ends. @todo Allocator pvPortMalloc
	cyclic_buf<unsigned int, ends_indexes_cb_def_size> end_indexes_cb;

	//! The cyclic buffer where the commands are held.
	cyclic_buf<char, N> cb;

  public:
	//! Returns the oldest command.
	std::string pop_command() noexcept;

	/**
	 * \brief Pushes an element to the buffer and returns true if the element indicates the end of command.
	 *
	 * The end of a command is indicated by '\r' character. '\n' are ignored.
	 */
	bool push_elem_and_check_command_end(char c) noexcept;
};

template <size_t N> std::string human_readable_commands_handler<N>::pop_command() noexcept
{
	// Firstly check whether there are lines in the buffer. If not then return immediately.
	if (end_indexes_cb.is_empty())
		return std::string("");

	auto beg = cb.tail;
	auto end = end_indexes_cb.pop_elem();

	unsigned int len;
	// Check whether there will be a swing.
	if (beg > end)
		len = cb.size - beg + end;
	else
		len = end - beg;

	if (len == 0)
	{
		// Perform a recursion. This makes this function return empty string only when there are no more lines
		// in the buffer.
		return pop_command();
	}

	// Get the string to be returned.
	std::string s(len, '\0');
	cb.pop_nelems(&s.front(), len);

	return s;
}

template <size_t N> bool human_readable_commands_handler<N>::push_elem_and_check_command_end(char c) noexcept
{
	// Ignore the line feed and null terminating character.
	if (c == '\n' || c == '\0')
		return false;

	// Treat the carriage return as the end of command.
	if (c == '\r')
	{
		end_indexes_cb.push_elem(cb.head);
		return true;
	}

	// Push any other character to the buffer.
	cb.push_elem(c);

	return false;
}
