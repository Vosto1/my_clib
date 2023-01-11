#include "test.h"

int main(void)
{
    //test_sequence();
    unsigned long long operations = 0;
    int tests = 100;
    printf("Running auto tests...\n");
    ticks start = now();
    for (int i = 0; i < tests; i++)
        operations += auto_tests(100, 10);
    ticks end = now();
    seconds secs = diff(start, end);
    printf("Computed %lld operations in %d tests during %f seconds.\n", operations, tests, secs);
    return 0;
}