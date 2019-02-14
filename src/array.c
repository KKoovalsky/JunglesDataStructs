/**
 * @file	array.c
 * @brief	Generic array handlers
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#include "array.h"

void *array_search(void *arr, size_t elem_size, size_t len, arr_search_checker v, void *match)
{
	if(!arr)
		return NULL;

	char *p = arr;
	for(unsigned i = 0; i < len; ++i)
	{
		if(v((void *) p, match))
			return p;
		p += elem_size;
	}

	return NULL;
}	
