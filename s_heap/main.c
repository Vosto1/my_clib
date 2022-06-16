#include "test.h"

int main(void)
{
    srand(time(NULL));
    test_sequence();
    compute_1_to_n_sequences_of_operations(1000000, INSERTION);
    compute_1_to_n_sequences_of_operations(1000000, DELETION);
    return heap_integrity_test(300) ? 0 : -1;
    return 0;
}