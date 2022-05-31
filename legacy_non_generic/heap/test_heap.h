#ifndef TEST_HEAP_H
#define TEST_HEAP_H

#include "heap.h"
#include <stdbool.h>
#include <assert.h>

typedef enum Test {
    INSERTION = 1,
    DELETION = 2,
}Test;

typedef Data Item;

void test_sequence();
void compute_1_to_n_sequences_of_operations(long n, Test type);
bool heap_integrity_test();

#endif