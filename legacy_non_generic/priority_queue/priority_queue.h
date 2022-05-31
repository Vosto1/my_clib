#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "./min_heap/heap.h"
#include "../utils/error.h"
#include "../utils/timer.h"

// data type -->
// data type is defined in data.h
#include "data.h"
// data type <--

typedef struct {
    heap h;
}PriorityQueue;

PriorityQueue createEmptyPriorityQueue();
ErrorCode initPriorityQueue(PriorityQueue* pq);
ErrorCode freePriorityQueue(PriorityQueue* pq);

int count(PriorityQueue* pq);
ErrorCode enqueuePQ(PriorityQueue* pq, Item item);
ErrorCode dequeuePQ(PriorityQueue* pq, Item* out);
bool trydequeuePQ(PriorityQueue* pq, Item* out);
bool peekPQ(PriorityQueue* pq, Item* out);

#endif
