#ifndef TEST_HEAP_H
#define TEST_HEAP_H

#define __TESTING__ // define to use testing functions

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "../s_dynamic_array/sdarray.h"
#include "../utils/timer.h"
#include "s_heap.h"
#include "../datatype.h"

typedef char Element;
typedef double Key;

typedef struct
{
    Key key;
    Element element;
} item;

typedef enum Test
{
    INSERTION = 1,
    DELETION = 2,
} Test;

// test functions
void test_sequence();
void compute_1_to_n_sequences_of_operations(long n, Test type);
bool heap_integrity_test();

#endif