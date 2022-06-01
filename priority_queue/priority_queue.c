#include "priority_queue.h"

PriorityQueue createEmptyPQ() {
    PriorityQueue pq;
    pq.h = s_createEmptyHeap();
    return pq;
}

size_t initPQ(PriorityQueue* pq, size_t size, int (*compare)(cvoidp x, cvoidp y)) {
    return s_initHeap(&pq->h, size, compare);
}

void freePQ(PriorityQueue* pq) {
    s_freeHeap(&pq->h);
    *pq = createEmptyPQ();
}

size_t count(PriorityQueue* pq) {
    return s_heapSize(&pq->h);
}

size_t enqueuePQ(PriorityQueue* pq, voidp item) {
    return s_heapInsert(&pq->h, item);
}

voidp dequeuePQ(PriorityQueue* pq) {
    if(count(pq) == 0) {
        errcset(EPQ_EMPTY);
        return NULL;
    }
    else return s_extractMin(&pq->h);
}

bool trydequeuePQ(PriorityQueue* pq, voidp* out) {
    if (count(pq) == 0) return false;
    else {
        *out = s_extractMin(&pq->h);
        return true;
    }
}

bool peekPQ(PriorityQueue* pq, voidp* out) {
    if (count(pq) == 0) return false;
    else {
        *out = s_peek(&pq->h);
        return true;
    }
}
