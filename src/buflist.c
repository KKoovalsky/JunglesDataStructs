/**
 * @file	buflist.c
 * @brief	List of buffers implementation file.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "buflist.h"
#include <string.h>
#include <stdlib.h>

#ifdef __FREERTOS__
#include "FreeRTOS.h"
#endif

struct buf_entry
{
	char *buf;
	size_t len;
};

void init_buflist(struct doubly_linked_list *l)
{
	dl_list_init(l, sizeof(struct buf_entry));
}

void add_buf_to_buflist(struct doubly_linked_list *l, const char *buf, size_t len)
{
	struct buf_entry be;

	// Allocate the new buffer 
	be.buf = alloc_mem(len);
	// And set its length
	be.len = len;
	// Then copy the content of 'buf' to the buffer
	memcpy(be.buf, buf, len);

	// Insert the new element on the end of the list
	dl_list_insert_at_tail(l, &be);
}

char *concat_buflist_and_rm(struct doubly_linked_list *l, size_t *len)
{
	if(!l->head)
	{
		*len = 0;
		return NULL;
	}

	size_t total_len = 0;
	for_each_in_dl_list(struct buf_entry, elem, (*l))
		total_len += elem->len;

	// Set the output len
	*len = total_len;
	
	// Allocate memory for a buffer which is a result of concatenation
	char *concat_buf = alloc_mem(total_len);

	// Concatenate only if allocation went successful
	if(concat_buf != NULL)
	{	
		size_t off = 0;
		for_each_in_dl_list(struct buf_entry, elem, (*l))
		{
			memcpy(concat_buf + off, elem->buf, elem->len);
			off += elem->len;
		}
	}
	
	// Remove the list even when could not allocate memory for the new buffer
	for_each_in_dl_list(struct buf_entry, elem, (*l))
		free_mem(elem->buf);
	dl_list_delete(l);
	init_buflist(l);

	return concat_buf;
}
