#include "test.h"

// I need to count collisions!

int main(void) {
    //test_sequence();
    unsigned long long operations = 0;
    int tests = 100;
    ticks start = now();
    for (int i = 0; i < tests; i++) {
        operations += auto_tests(1000, 100);
    }
    ticks end = now();
    seconds secs = diff(start, end);
    printf("Computer %d operations in %d tests during %f seconds.\n", operations, tests, secs);
    return 0;
}