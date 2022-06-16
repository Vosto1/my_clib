#include "timer.h"

ticks now()
{
    return clock();
}

seconds diff(ticks start, ticks end)
{
    clock_t diffcycles = end - start;
    double clock_s = (double)diffcycles/(double)CLOCKS_PER_SEC;

    return clock_s;
}
