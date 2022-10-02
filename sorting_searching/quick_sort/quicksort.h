#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "../../datatype.h"

void quicksort(voidp_t array[], dim_t array_size, int (*compare)(voidp_t, voidp_t));

#endif