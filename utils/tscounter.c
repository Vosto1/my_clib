#include "tscounter.h"

counter new_counter()
{
    counter c;
    c.count = 0;
    return c;
}

void increment(counter *c)
{
    c->count++;
}

void reset(counter *c)
{
    c->count = 0;
}

uint count(counter c)
{
    return c.count;
}