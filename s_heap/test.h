#ifndef TEST_HEAP_H
#define TEST_HEAP_H

#define __TESTING__ // define to use testing functions

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "../dynamic_array/dynamic_array.h"
#include "../utils/timer.h"
#include "s_heap.h"

typedef char Element;
typedef double Key;

typedef struct
{
    Key key;
    Element element;
} Item;

typedef unsigned long long size_t;

typedef enum Test
{
    INSERTION = 1,
    DELETION = 2,
} Test;
// user defined functions needed for the s_heap
static int compare(const void *x, const void *y);
static void heapPrintTree(s_heap *h);
// test functions
void test_sequence();
void compute_1_to_n_sequences_of_operations(long n, Test type);
bool heap_integrity_test();

#endif