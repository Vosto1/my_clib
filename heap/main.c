#include "test.h"

#define PASSED 0
#define NOT_PASSED -1

int main(void)
{
    test_sequence();
    compute_1_to_n_sequences_of_operations(10000, INSERTION);
    compute_1_to_n_sequences_of_operations(10000, DELETION);
    return heap_integrity_test(100) ? PASSED : NOT_PASSED;
}
