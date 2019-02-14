/**
 * @file	buflist.h
 * @brief	List of buffers handlers
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef BUFLIST_H
#define BUFLIST_H

#include "doubly_linked_list.h"

/*! \brief Function which initializes a list of buffers
 *
 *  This function must be called before using any of the function operating on the list of buffers.
 *  Note that list of buffers is of the type struct doubly_linked_list.
 *
 * \param[in]	Address of a list of buffers
 */
void init_buflist(struct doubly_linked_list *l);

/*! \brief	Add a buffer to a list of buffers
 *
 *  This function creates a copy of the passed buffer (the copy is dynamically allocated) and adds 
 *  the new buffer to the list.
 *
 * \param[in] l		An address of a list of buffers
 * \param[in] buf	The buffer which will be added to the list of buffers
 * \param[in] len	The legth of the buffer (number of elements in the buffer)
 */
void add_buf_to_buflist(struct doubly_linked_list *l, const char *buf, size_t len);

/*! \brief Concatenate all of the buffers from a list into one and cleanup the list.
 *
 *  Note that the newly allocated memory for the returned pointer to the new buffer must be freed later.
 *  The list is destructed after a call to this function.
 *
 * \param[in] l		An address of a list of buffers
 * \param[out] len  Pointer to a variable where the length of the newly created buffer will be held
 * \return			Pointer to the buffer which is a concatenation of the buffers which were added to the list.
 *					NULL if could not allocate memory for the new buffer.
 */
char *concat_buflist_and_rm(struct doubly_linked_list *l, size_t *len);

#endif /* BUFLIST_H */
