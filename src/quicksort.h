#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "datatype.h"

void quicksort(void* array[], uint array_size, int (*compare)(const void*, const void*));

#endif