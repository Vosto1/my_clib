#include "llqueue.h"

llqueue llq_create_new_empty()
{
    llqueue q;
    q = ll_create_empty();
    return q;
}

bool llq_is_empty(llqueue *q)
{
    return q == NULL;
}

void llq_enqueue(llqueue *q, cvoidp_t item)
{
    ll_add_last(q, item);
}

cvoidp_t llq_dequeue(llqueue* q)
{
    return ll_remove_last(q);
}

cvoidp_t llq_peek(llqueue* q)
{
    return ll_get_first(q);
}

dim_t llq_count(llqueue* q)
{
    return ll_node_count(q);
}