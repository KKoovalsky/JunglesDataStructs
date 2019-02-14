/**
 *  \file	doubly_linked_list.h
 *  \brief	Declarations and type definitions for doubly linked list.
 *  \author Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#ifdef __FREERTOS__
#ifndef alloc_mem
#define alloc_mem		pvPortMalloc
#endif
#ifndef free_mem
#define free_mem		vPortFree
#endif
#else
#ifndef alloc_mem
#define alloc_mem		malloc
#endif
#ifndef free_mem
#define free_mem		free
#endif
#endif

#include <stdio.h>

/**
 * Iterates through a doubly linked list which holds element of the type \a T in the list \a l (passed by the name).
 * Each element can be accessed in the loop using \a element pointer.
 */
#define for_each_in_dl_list(T, element, list) for(struct dll_node *n = list.head; n; n = n->next) \
													for(T *element = n->elem; element; element = NULL)
/**
 * \brief A node of a generic doubly linked list
 */
struct dll_node
{
	struct dll_node *next, *prev;

	//! Pointer to the element being held by the node.
	void *elem;
};

/**
 * \brief A generic doubly linked list
 */
struct doubly_linked_list
{
	struct dll_node *head, *tail;
	// ! Size of a single element held in the list
	size_t elem_size;
};

/**
 *  \brief Initialize list pointers with NULL values and initialize size of an element held by a node
 *
 *  This is obligatory to firstly call this function before calling any other functions which operates on a list.
 *  
 *  \param[in] l				Address of a list which is going to be initialized.
 *  \param[in] elem_size		Size of a single element held in the list (most probably sizeof(struct struct_name) 
 *								will be used here)
 */
void dl_list_init(struct doubly_linked_list *l, size_t elem_size);

/**
 *  \brief Insert an element on the end of a list
 *
 *  The element is copy constructed in place. The memory for the node and the element is allocated at once.
 *  The allocated memory is logically divided into two memory blocks.
 *  The first block of memory contains struct dll_node and the second block contains the copied element.
 *  elem pointer of struct dll_node is set to point on the second block of memory.
 *
 *	\param[in] l			Address of a list to which the element will be inserted.
 *	\param[in] elem			Pointer to the element which will be added to the list.
 *	\return					Pointer to the newly created node, NULL if couldn't allocate memory.  
 */
struct dll_node * dl_list_insert_at_tail(struct doubly_linked_list *l, void *elem);

/**
 *	\brief Remove a node from a list
 *
 *	\param[in] l	Address of a list from which the node will be removed.
 *	\param[in] n	Pointer to the node which will be removed
 */
void dl_list_remove_node(struct doubly_linked_list *l, struct dll_node *n);

/**
 *  \brief List destructor
 */
void dl_list_delete(struct doubly_linked_list *l);

#endif /* APP_INC_DOUBLY_LINKED_LIST_H_ */
