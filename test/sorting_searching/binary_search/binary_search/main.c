#include "test.h"
#include "timer.h"

int main(void)
{
    ticks start = now();
    test(10000);
    ticks end = now();
    printf("Time: %.2f\n", diff(start, end));
    return 0;
}