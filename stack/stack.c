#include "stack.h"

stack st_create_empty()
{
    stack s = sda_create_empty();
    return s;
}

size_t st_init(stack* s, size_t init_size, void (*freeObject)(voidp_t))
{
    sda_init(s, init_size, freeObject);
}

size_t st_count(stack *s)
{
    return sda_count(s);
}

size_t st_clear(stack *s)
{
    return sda_clear(s);
}

cvoidp_t st_peek(stack *s)
{
    return sda_at(s, st_count(s) - 1);
}

bool st_push(stack *s, cvoidp_t item)
{
    return sda_insert(s, item) > 0;
}

voidp_t st_pop(stack *s)
{
    return sda_remove_last(s);
}