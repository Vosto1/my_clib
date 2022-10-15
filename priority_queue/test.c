#include "test.h"

typedef int priority;
typedef char element;

typedef struct
{
    priority p;
    element e;
} item;

int compare(cvoidp_t x, cvoidp_t y)
{
    item *item1 = (item *)x;
    item *item2 = (item *)y;
    return item1->p - item2->p;
}

void print(size_t used, voidp_t a[])
{
    item *itemArr = (item *)a;
    for (int i = 0; i < used; i++)
    {
        printf("value at %d: [%d;%c]\n", i, itemArr[i].p, itemArr[i].e);
    }
    printf("\n");
}

item *create_item(int priority)
{
    item *it = (item *)malloc(sizeof(item));
    it->p = priority;
    it->e = (char)priority;
    return it;
}

void freeObject(voidp_t i)
{
    free(i);
}

void heapPrintTree(sheap *h)
{
    int y = 0;
    int x = 0;
    for (int i = 0; i < sh_size(h); i++)
    {
        for (int j = 0; j < pow(2, i) && j + pow(2, i) <= sh_size(h); j++)
        {
            x = j + (int)pow(2, i) - 1;
            y = h->items.used;
            if (y > x)
            {
                item *it = (item *)h->items.array[x];
                printf("[k%d|%c]", it->p, it->e);
            }
            else
                printf("----");
        }
        printf("\n");
    }
}

bool integrity_check(int n)
{
    srand(time(NULL));
    priorityqueue pq = pq_create_empty();
    assert(pq_init(&pq, 1, &compare, &freeObject) == 1);
    item *it;
    item *out;
    item *min;
    int result = 0; // must be initialized to 0
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
            { // 80%
                it = create_item(val);
                assert(pq_enqueue(&pq, (void *)it) != 0);
                // inc enqs
                enqcount++;
            }
            else if (val > 80 && val < 88)
            { // 20%
                if (pq_count(&pq) != 0)
                {
                    min = sh_peek(&pq.h);
                    it = pq_dequeue(&pq);
                    assert(it != NULL);
                    result = compare(min, it);
                    free(it);
                    // inc dqs
                    dqcount++;
                }
            }
            else if (val > 87 && val < 95)
            {
                min = sh_peek(&pq.h);
                if (pq_trydequeue(&pq, (void *)&out))
                {
                    assert(out != NULL);
                    result = compare(min, out);
                    free(out);
                }
                // inc tdqs
                tdqcount++;
            }
            else
            { // if (val > 94)
                min = sh_peek(&pq.h);
                if (pq_peek(&pq, (void *)&out))
                {
                    result = compare(min, out);
                }
                // inc peeks
                pcount++;
            }
            if (!sh_test_heap_integrity(&pq.h))
            {
                system("clear");
                printf("priorityqueue: heap integrity broken\n");
                printf("Structure at error:\n");
                heapPrintTree(&pq.h);
                return false;
            }
            else if (result != 0)
            {
                system("clear");
                printf("priorityqueue: dequeue wrong result.\n");
                printf("Structure at error:\n");
                heapPrintTree(&pq.h);
                return false;
            }
            // inc test counter
            totalTests++;
        }
        end = now();
        printf("Computed %d operations (%d enqueues, %d dequeues %d trydequeue and %d peeks) during %f seconds, passed integrity test.\n", x, enqcount, dqcount, tdqcount, pcount, diff(start, end));
        // reset counters
        dqcount = 0;
        enqcount = 0;
        tdqcount = 0;
        pcount = 0;
    }
    ticks testEnd = now();
    seconds passed = diff(testStart, testEnd);
    printf("Computed a total of %ld operations and integrity tests.\n", totalTests);
    printf("Total test running time: %fs\n", passed);
    printf("Integrity test exiting...\n");
    assert(pq_free(&pq));
    return true;
}