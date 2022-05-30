#include "test_heap.h"
#include "../utils/timer.h"

Item createItem(int x) {
    Item item;
    item.element = (char)x;
    item.key = x;
    return item;
}

Item randomElement(heap* h) {
    int index = rand() % h->items.used;
    return h->items.array[index];
}

void compute_1_to_n_sequences_of_operations(long n, Test type) {
    ErrorCode e;
    heap h = createEmptyHeap();
    e = initHeap(&h, 1);
    errorHandler(e);
    long j = 1;
    Item item;
    Item toDelete;
    ticks start;
    ticks end;
    switch (type) {
        case INSERTION:
            while (j <= n) {
                start = now();
                for (int i = 0; i < j; i++) {
                    int val = rand() % 1000;
                    item = createItem(val);
                    heapInsert(&h, item);
                }
                end = now();
                printf("Computed %d insertion operations during %f seconds.\n", j, diff(start, end));
                j *= 2;
            }
        break;
        case DELETION:
            while (j <= n) {
                for (int i = 0; i < j; i++) {
                    int val = rand() % 1000;
                    item = createItem(val);
                    heapInsert(&h, item);
                }

                start = now();
                for (int i = 0; i < j; i++) {
                    toDelete = randomElement(&h);
                    heapRemove(&h, toDelete);
                }
                end = now();
                printf("Computed %d deletion operations during %f seconds.\n", j, diff(start, end));
                j *= 2;
            }
        break;
    }
    e = freeHeap(&h);
    errorHandler(e);
}

bool heap_integrity_test(int n) {
    srand(time(NULL));
    heap h = createEmptyHeap();
    ErrorCode e = initHeap(&h, 10);
    errorHandler(e);
    Item item;
    Item toDelete;
    ticks start;
    ticks end;
    // counters
    int delcount = 0, inscount = 0;
    long totalTests = 0;

    // check
    ticks testStart = now();
    for (int x = 1; x < n + 1; x++)
    {
        start = now();
        for (int i = 0; i < x; i++)
        {
            int val = rand() % 100;
            if (val < 81)
            { //80%
                item = createItem(val);
                heapInsert(&h, item);
                // inc counter
                inscount++;
            }
            else
            { //20%
                if (h.items.used != 0) {
                    toDelete = randomElement(&h);
                    heapRemove(&h, toDelete);
                    // inc counter
                    delcount++;
                } 
            }
            if(!testHeapIntegrity(&h)){
                system("clear");
                printf("Heap integrity broken\n");
                heapPrintTree(&h);
                return false;
                //exit(-1);
            }
            // inc test counter
            totalTests++;
        }
        end = now();
        printf("Computed %d operations (%d insertions and %d deletions) during %f seconds, passed integrity check.\n", x, inscount, delcount, diff(start, end));
        // reset counters
        delcount = 0;
        inscount = 0;
    }
    ticks testEnd = now();
    seconds passed = diff(testStart, testEnd);
    printf("Computed a total of %ld operations and tests.\n", totalTests);
    printf("Total test running time: %fs\n", passed);
    printf("Integrity test exiting...\n");
    e = freeHeap(&h);
    errorHandler(e);
    return true;
}

void test_sequence() {
    srand(time(NULL));
    heap h = createEmptyHeap();
    ErrorCode e = initHeap(&h, 10);
    errorHandler(e);
    ticks programStart = now();
    ticks start;
    ticks end;

    Item item0 = createItem(97);
    Item item1 = createItem(82);
    Item item2 = createItem(59);
    Item item3 = createItem(99);
    Item item4 = createItem(77);
    Item item5 = createItem(34);
    Item item6 = createItem(78);
    Item item7 = createItem(46);
    Item item8 = createItem(21);
    Item item9 = createItem(45);

    heapInsert(&h, item0);
    heapInsert(&h, item1);
    heapInsert(&h, item2);
    heapInsert(&h, item3);
    heapInsert(&h, item4);
    heapInsert(&h, item5);

    assert(testHeapIntegrity(&h));

    printf("print 1\n");
    heapPrintTree(&h);

    assert(compareData(_min(&h), item5) == 0);

    e = decreaseKey(&h, item3, createItem(1));
    errorHandler(e);

    // decrease key of item3 to 1 but still same element as item3
    Item tmp;
    tmp.key = 1;
    tmp.element = item3.element;
    
    assert(compareData(extractMin(&h), tmp) == 0);

    assert(heapRemove(&h, item2));

    heapInsert(&h, item6);
    heapInsert(&h, item7);
    heapInsert(&h, item8);
    heapInsert(&h, item9);

    assert(testHeapIntegrity(&h));

    printf("print 2\n");
    heapPrintTree(&h);

    assert(compareData(_min(&h), item8) == 0);
    assert(compareData(extractMin(&h), item8) == 0);
    assert(!heapRemove(&h, item8));

    assert(compareData(extractMin(&h), item5) == 0);
    assert(compareData(extractMin(&h), item9) == 0);
    assert(compareData(extractMin(&h), item7) == 0);
    assert(compareData(extractMin(&h), item4) == 0);
    assert(compareData(extractMin(&h), item6) == 0);
    assert(compareData(extractMin(&h), item1) == 0);
    assert(compareData(extractMin(&h), item0) == 0);

    e = freeHeap(&h);
    errorHandler(e);

    h = createEmptyHeap();
    e = initHeap(&h, 1);
    errorHandler(e);

    Item b[10] = {item0, item1, item2, item3, item4, item5, item6, item7, item8, item9};

    h = buildMinHeap(b, 10);

    printf("build min heap\n");
    heapPrintTree(&h);

    assert(testHeapIntegrity(&h));

    e = freeHeap(&h);
    errorHandler(e);
    printf("Tests passed.\n");
}