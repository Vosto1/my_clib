#include "priority_queue.h"

priorityqueue pq_create_empty()
{
    priorityqueue pq;
    pq.h = sh_create_empty();
    return pq;
}

int pq_init(priorityqueue *pq, uint size, int (*compare)(const void* x, const void* y), void (*freeObject)(void*))
{
    return sh_init(&pq->h, size, compare, freeObject);
}

bool pq_free(priorityqueue *pq)
{
    bool r = sh_free(&pq->h);
    *pq = pq_create_empty();
    return r;
}

bool pq_destroy(priorityqueue *pq)
{
    bool r = sh_destroy(&pq->h);
    *pq = pq_create_empty();
    return r;
}

int pq_count(priorityqueue *pq)
{
    return sh_size(&pq->h);
}

int pq_enqueue(priorityqueue *pq, void* item)
{
    return sh_insert(&pq->h, item);
}

void* pq_dequeue(priorityqueue *pq)
{
    if (pq_count(pq) == 0)
    {
        return NULL;
    }
    else
        return sh_extract_min(&pq->h);
}

bool pq_trydequeue(priorityqueue *pq, void* *out)
{
    if (pq_count(pq) == 0)
        return false;
    else
    {
        *out = sh_extract_min(&pq->h);
        return true;
    }
}

bool pq_peek(priorityqueue *pq, void* *out)
{
    if (pq_count(pq) == 0)
        return false;
    else
    {
        *out = sh_peek(&pq->h);
        return true;
    }
}
