#include "priority_queue.h"

PriorityQueue createEmptyPriorityQueue() {
    PriorityQueue pq;
    pq.h = createEmptyHeap();
    return pq;
}

ErrorCode initPriorityQueue(PriorityQueue* pq) {
    return initHeap(&pq->h, 1);
}

ErrorCode freePriorityQueue(PriorityQueue* pq) {
    ErrorCode e = freeHeap(&pq->h);
    *pq = createEmptyPriorityQueue();
    return e;
}

int count(PriorityQueue* pq) {
    return heapSize(&pq->h.items);
}

ErrorCode enqueuePQ(PriorityQueue* pq, Item item) {
    return heapInsert(&pq->h, item);
}

ErrorCode dequeuePQ(PriorityQueue* pq, Item* out) {
    if(count(pq) == 0) return PQ_EMPTY;
    else {
        *out = extractMin(&pq->h);
        return SUCCESS;
    }
}

bool trydequeuePQ(PriorityQueue* pq, Item* out) {
    if (count(pq) == 0) return false;
    else {
        *out = extractMin(&pq->h);
        return true;
    }
}

bool peekPQ(PriorityQueue* pq, Item* out) {
    if (count(pq) == 0) return false;
    else {
        *out = _min(&pq->h);
        return true;
    }
}
