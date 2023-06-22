#include "stack.h"

stack st_create_empty()
{
    stack s = mda_create_empty();
    return s;
}

int st_init(stack *s, uint init_size, void (*freeObject)(void *))
{
    return mda_init(s, init_size, freeObject);
}

uint st_count(stack *s)
{
    return mda_count(s);
}

int st_clear(stack *s)
{
    return mda_clear(s);
}

const void *st_peek(stack *s)
{
    return mda_at(s, st_count(s) - 1);
}

bool st_push(stack *s, const void *item)
{
    return mda_insert(s, (void*)item) > 0;
}

void *st_pop(stack *s)
{
    return mda_remove_last(s);
}

bool st_is_empty(stack* s)
{
    return mda_is_empty(s);
}

bool st_free(stack* s)
{
    return mda_free(s);
}

bool st_destroy(stack* s)
{
    return mda_destroy(s);
}