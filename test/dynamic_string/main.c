#include "test.h"

int main()
{
    // remove seeding before debugging!
    ticks start = now();
    srand(time(0));
    test();
    auto_tests(10000);
    ticks end = now();
    seconds time_passed = diff(start, end);
    printf("%.2fs\n", time_passed);
    return 0;
}