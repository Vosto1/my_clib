#ifndef TEST_HEAP_H
#define TEST_HEAP_H

#define _HEAP_TESTING_ // define heap testing to use testing functions

#include <stdbool.h>
#include <assert.h>
#include "s_heap.h"
#include "../utils/timer.h"

// datatype -->
#include "data.h"
// datatype <--

typedef enum Test {
    INSERTION = 1,
    DELETION = 2,
}Test;

void test_sequence();
void compute_1_to_n_sequences_of_operations(long n, Test type);
bool heap_integrity_test();

#endif