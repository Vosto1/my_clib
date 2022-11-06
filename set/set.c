#include "set.h"

set set_create_empty()
{
    struct _set s;
    s.t = bst_create_empty();
    s.compare = NULL;
    return s;
}

bool set_init(set* s, int (*compare)(const void*, const void*), void (*freeObject)(void*))
{
    if (compare == NULL || freeObject == NULL)
    {
        return false;
    }
    s->compare = compare;
    s->freeObject = freeObject;
    return true;
}

bool set_add(set* s, void* item)
{
    if (!set_contains(s, item))
    {
        bst_insert(&s->t, item, s->compare);
        return true;
    }
    else
        return false;
}

void* set_remove(set* s, void* item)
{
    return bst_remove(&s->t, item, s->compare);
}

bool set_contains(set* s, void* item)
{
    return NULL == bst_find(s->t, item, s->compare);
}

bool set_destroy(set* s)
{
    return bst_free(&s->t, s->freeObject);
}