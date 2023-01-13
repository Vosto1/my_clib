#include "stack.h"

// this should never be called
static int compare(const void* a, const void* b) // maybe extend the stack so that you can check whats inside (e.g. st_exists())
{
    printf("Error! You used a function within darray which uses the compare function.");
    exit(EXIT_FAILURE);
}

stack st_create_empty()
{
    stack s = da_create_empty();
    return s;
}

int st_init(stack *s, uint init_size, void (*freeObject)(void *))
{
    return da_init(s, init_size, &compare, freeObject);
}

uint st_count(stack *s)
{
    return da_count(s);
}

int st_clear(stack *s)
{
    return da_clear(s);
}

const void *st_peek(stack *s)
{
    return da_at(s, st_count(s) - 1);
}

bool st_push(stack *s, const void *item)
{
    return da_insert(s, (void*)item) > 0;
}

void *st_pop(stack *s)
{
    return da_remove_last(s);
}