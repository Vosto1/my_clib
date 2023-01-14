#include "stack.h"

stack st_create_empty()
{
    stack s = sda_create_empty();
    return s;
}

int st_init(stack *s, uint init_size, void (*freeObject)(void *))
{
    return sda_init(s, init_size, freeObject);
}

uint st_count(stack *s)
{
    return sda_count(s);
}

int st_clear(stack *s)
{
    return sda_clear(s);
}

const void *st_peek(stack *s)
{
    return sda_at(s, st_count(s) - 1);
}

bool st_push(stack *s, const void *item)
{
    return sda_insert(s, (void*)item) > 0;
}

void *st_pop(stack *s)
{
    return sda_remove_last(s);
}

bool st_is_empty(stack* s)
{
    return sda_is_empty(s);
}

bool st_free(stack* s)
{
    return sda_free(s);
}

bool st_destroy(stack* s)
{
    return sda_destroy(s);
}