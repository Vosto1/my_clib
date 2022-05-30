#include "test_priority_queue.h"

Item createItem(int priority) {
    Item item;
    item.p = priority;
    item.e = (char)priority;
    return item;
}

/* Item randomElement(PriorityQueue* pq) {
    int index = rand() % count(pq);
    return pq->h.items.array[index];
} */

bool integrity_check(int n) {
    srand(time(NULL));
    PriorityQueue pq = createEmptyPriorityQueue();
    ErrorCode e = initPriorityQueue(&pq);
    errorHandler(e);
    Item item;
    Item toDequeue;
    Item out;
    Item min;
    int result = 0; // must be initialized to 0
    bool operation;
    ticks start;
    ticks end;

    // counters
    int dqcount = 0, enqcount = 0, tdqcount = 0, pcount = 0;
    long totalTests = 0;

    // check
    ticks testStart = now();
    for (int x = 1; x < n + 1; x++)
    {
        start = now();
        for (int i = 0; i < x; i++)
        {
            int val = (rand() % 100) + 1; // 1-100
            if (val < 81)
            { //80%
                item = createItem(val);
                e = enqueuePQ(&pq, item);
                errorHandler(e);
                // inc enqs
                enqcount++;
            }
            else if(val > 80 && val < 88)
            { //20%
                if (count(&pq) != 0) {
                    min = _min(&pq.h);
                    e = dequeuePQ(&pq, &out);
                    errorHandler(e);
                    result = compareData(min, out);
                    // inc dqs
                    dqcount++;
                } 
            }
            else if(val > 87 && val < 95) {
                min = _min(&pq.h);
                if(trydequeuePQ(&pq, &out)) {
                    result = compareData(min, out);
                }
                // inc tdqs
                tdqcount++;
            }
            else { // if (val > 94)
                min = _min(&pq.h);
                if(peekPQ(&pq, &out)) {
                    result = compareData(min, out);
                }
                // inc peeks
                pcount++;
            }
            if(!testHeapIntegrity(&pq.h)){
                system("clear");
                printf("PriorityQueue: heap integrity broken\n");
                printf("Structure at error:\n");
                heapPrintTree(&pq.h);
                return false;
            } else if (result != 0) {
                system("clear");
                printf("PriorityQueue: dequeue wrong result.\n");
                printf("Structure at error:\n");
                heapPrintTree(&pq.h);
                return false;
            }
            // inc test counter
            totalTests++;
        }
        end = now();
        printf("Computed %d operations (%d enqueues, %d dequeues %d trydequeue and %d peeks) during %f seconds, passed integrity check.\n", x, enqcount, dqcount, tdqcount, pcount, diff(start, end));
        // reset counters
        dqcount = 0;
        enqcount = 0;
        tdqcount = 0;
        pcount = 0;
    }
    ticks testEnd = now();
    seconds passed = diff(testStart, testEnd);
    printf("Computed a total of %ld operations and tests.\n", totalTests);
    printf("Total test running time: %fs\n", passed);
    printf("Integrity test exiting...\n");
    e = freePriorityQueue(&pq);
    errorHandler(e);
    return true;
}