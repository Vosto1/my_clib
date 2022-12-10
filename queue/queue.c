#include "queue.h"

static uint q_item_count(queue* q)
{
    return q->end - q->front;
}

queue q_create_new_empty()
{
    queue q;
    q.freeObject = NULL;
    q.queue = NULL;
    q.end = 0;
    q.front = 0;
    q.size = 0;
    return q;
}

size_t q_init(queue *q, size_t size, void (*freeObject)(void*))
{
    q->freeObject = freeObject;
    q->queue = (void**)malloc(sizeof(void*)*size);
    q->size = size;
}

bool q_is_empty(queue* q)
{
    return q->queue == NULL;
}

bool q_enqueue(queue* q, const void* item)
{
    if (q_is_empty(q))
    {
        return false;
    }

    if (q->size == q->end)
    {
        q->size *= 2;
        void** temp = (void**)realloc(q->queue, sizeof(void*)*q->size);
        if (temp != NULL)
        {
            q->queue = temp;
        }
        else
        {
            q->size /= 2;
            return false;
        }
    }
    q->end += 1;
    q->queue[q->end - 1] = item;
    return true;
}

void* q_dequeue(queue* q)
{
    if (q_is_empty(q) || q_item_count(q) <= 0)
    {
        return false;
    }

    double ratio = (double)(q->end - q->front)/q->size;
    if (ratio <= QUARTER && q->size != 1)
    {
        //move all items to the front of the memory
        uint icount = q_item_count(q);
        uint f = q->front;
        for (uint i = 0; i < icount; i++)
        {
            q->queue[i] = q->queue[i+f];
        }
        q->front = 0;
        q->end = icount;
        q->size /= 2;
        void** temp = (void**)realloc(q->queue, sizeof(void*)*q->size);
        if (temp != NULL)
        {
            q->queue = temp;
        }
        else
        {
            q->size /= 2;
            return NULL;
        }
    }
    void* item = q->queue[q->front];
    q->front += 1;
    return item;
}

const void* q_peek(queue* q)
{
    return q->queue[q->end];
}

bool q_destroy(queue* q)
{
    if (q_is_empty(q))
    {
        return false;
    }
    for (uint i = 0; i < q_item_count(q); i++)
    {
        (*q->freeObject)(q->queue[i+q->front]);
    }
    return true;
}