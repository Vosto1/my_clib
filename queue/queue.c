#include "queue.h"

struct q {
    voidp_t *array;
    size_t size;
    size_t used;
    //hashtable rules; // index rules
    size_t first_element;
    size_t last_element;
};

/* struct r {
    size_t index_from;   // if you are at this index
    size_t index_to;     // then this is the previous index
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
