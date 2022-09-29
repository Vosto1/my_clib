#include "queue.h"

struct q {
    voidp_t *array;
    dim_t size;
    dim_t used;
    //hashtable rules; // index rules
    dim_t first_element;
    dim_t last_element;
};

/* struct r {
    dim_t index_from;   // if you are at this index
    dim_t index_to;     // then this is the previous index
}; */

queue q_create_new_empty()
{
    queue q;
    q.array = NULL;
    q.first_element = 0;
    q.last_element = 0;
    //q.rules = ht_create_empty();
    q.size = 0;
    q.used = 0;
    return q;
}

bool q_is_empty(queue *q)
{
    return q->used == 0;
}

bool q_enqueue(queue *q, cvoidp_t item)
{
    return false;
}
