#include "priority_queue.h"

priorityqueue pq_create_empty()
{
    priorityqueue pq;
    pq.h = sh_create_empty();
    return pq;
}

dim_t pq_init(priorityqueue *pq, dim_t size, int (*compare)(cvoidp_t x, cvoidp_t y))
{
    return sh_init(&pq->h, size, compare);
}

void pq_free(priorityqueue *pq)
{
    sh_free(&pq->h);
    *pq = pq_create_empty();
}

dim_t pq_count(priorityqueue *pq)
{
    return sh_size(&pq->h);
}

dim_t pq_enqueue(priorityqueue *pq, voidp_t item)
{
    return sh_insert(&pq->h, item);
}

voidp_t pq_dequeue(priorityqueue *pq)
{
    if (pq_count(pq) == 0)
    {
        errcset(EPQ_EMPTY);
        return NULL;
    }
    else
        return sh_extract_min(&pq->h);
}

bool pq_trydequeue(priorityqueue *pq, voidp_t *out)
{
    if (pq_count(pq) == 0)
        return false;
    else
    {
        *out = sh_extract_min(&pq->h);
        return true;
    }
}

bool pq_peek(priorityqueue *pq, voidp_t *out)
{
    if (pq_count(pq) == 0)
        return false;
    else
    {
        *out = sh_peek(&pq->h);
        return true;
    }
}
