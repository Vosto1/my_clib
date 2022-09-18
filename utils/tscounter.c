#include "tscounter.h"


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

counter new_counter()
{
    counter c;
    c.count = 0;
    c.CNT = count;
    c.RST = reset;
    c.INC = increment;
    return c;
}