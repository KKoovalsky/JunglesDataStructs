/**
 *  @file	doubly_linked_list.c
 *  @brief	Implementation of doubly linked list
 *  @author Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "doubly_linked_list.h"
#include <string.h>
#include <stdlib.h>

#ifdef __FREERTOS__
#include "FreeRTOS.h"
#endif

void dl_list_init(struct doubly_linked_list *l, size_t elem_size)
{
	l->head = l->tail = NULL;
	l->elem_size = elem_size;
}

static struct dll_node * new_node(struct doubly_linked_list *l, void *elem)
{
	// Allocate memory just once for the node and the element simultaneously
	struct dll_node *n = (struct dll_node *) alloc_mem(sizeof(struct dll_node) + l->elem_size);
	// Set the pointer to the memory block after the node
	n->elem = n + 1;
	// Copy content of the element to the list record
	memcpy(n->elem, elem, l->elem_size);
	n->prev = n->next = NULL;
	return n;
}

struct dll_node * dl_list_insert_at_tail(struct doubly_linked_list *l, void *elem)
{
	if(l->head == NULL)
	{
		l->head = l->tail = new_node(l, elem);
		return l->tail;
	}

	l->tail->next = new_node(l, elem);
	l->tail->next->prev = l->tail;
	l->tail = l->tail->next;
	return l->tail;
}

void dl_list_remove_node(struct doubly_linked_list *l, struct dll_node *n)
{
	if(l->head == NULL || n == NULL)
		return;

	if(l->head == l->tail && n == l->head)
	{
		l->head = l->tail = NULL;
		free_mem(n);
		return;
	}

	if(n == l->head)
	{
		l->head = n->next;
		l->head->prev = NULL;
		free_mem(n);
		return;
	}

	if(n == l->tail)
	{
		l->tail = l->tail->prev;
		l->tail->next = NULL;
		free_mem(n);
		return;
	}

	n->prev->next = n->next;
	n->next->prev = n->prev;
	free_mem(n);
}

void dl_list_delete(struct doubly_linked_list *l)
{
	while(l->head)
		dl_list_remove_node(l, l->head);
	l->head = l->tail = NULL;
}
