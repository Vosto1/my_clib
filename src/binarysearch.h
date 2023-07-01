#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include <stdio.h>
#include "datatype.h"
#include "stack.h"

/**
 * @brief 
 * 
 * @param array sorted array to search in
 * @param element element to look for
 * @param size size of array
 * @param compare function that can compare two elements
 * @return int -1 if not present otherwise index of the element
 */
int binary_search(void* array[], void* element, uint size, int (*compare)(const void* element_to_search_for, const void* candidate_element));

#endif