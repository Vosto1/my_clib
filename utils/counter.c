#include "counter.h"

uint counter;

void increment()
{
    counter++;
}

void reset()
{
    counter = 0;
}

uint count()
{
    return counter;
}