/**
 * @file	array.h
 * @brief	Generic array handlers
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */
#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stdio.h>

/**
 * @brief	Type which determines a checker signature which is used for each element in the searched array
 *
 *			A function with this signature should return true if the first argument equals (in user defined
 *			rules) the second argument, false otherwise.
 */
typedef bool (*arr_search_checker)(void *, void *);

/**
 * @brief	Function used for arrays to search for a match
 *
 *			In C++ one would not use the checker function as only could override '==' operator and use '=='
 *			operator on each element. Then the checker could be removed. The checker is kind of replacement of
 *			this C++ feature.
 *
 * @param[in] arr		Pointer to an array
 * @param[in] elem_size	Size of single element in the array (use sizeof for that parameter)
 * @param[in] len		Length of the array (number of elements in the array)
 * @param[in] v			Pointer to a function which checks whether a match is found @see arr_search_checker
 * @param[in] match		Pointer to an element which is a match
 * @return Pointer to the matched record or NULL if it has been not found.
 */
void *array_search(void *arr, size_t elem_size, size_t len, arr_search_checker v, void *match);

#endif /* ARRAY_H */
