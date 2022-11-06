#include "set.h"

set set_create_empty()
{
    struct _set s;
    s.t = bst_create_empty();
    s.compare = NULL;
    return s;
}

bool set_init(set* s, int (*compare)(void*, void*), void (*freeObject)(void*))
{
    if (compare == NULL || freeObject == NULL)
    {
        return false;
    }
    s->compare = compare;
    s->freeObject = freeObject;
    return true;
}

bool set_add(set* s, voidp_t item)
{
    if (!set_contains(s, item))
    {
        bst_insert(&s->t, item, s->compare);
        return true;
    }
    else
        return false;
}

voidp_t set_remove(set* s, voidp_t item)
{
    return bst_remove(&s->t, item, s->compare);
}

bool set_contains(set* s, voidp_t item)
{
    return NULL == bst_find(s->t, item, s->compare);
}

bool set_destroy(set* s)
{
    return bst_free(s->t, s->freeObject);
}