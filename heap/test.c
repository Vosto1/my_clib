#include "test.h"

Item* createItem(int x) {
    Item* item = (Item*)malloc(sizeof(Item));
    item->element = (char)x;
    item->key = x;
    return item;
}

Item* randomElement(heap* h) {
    int index = rand() % h->items.used;
    return h->items.array[index];
}

/* void compute_1_to_n_sequences_of_operations(long n, Test type) {
    heap h = createEmptyHeap();
    initHeap(&h, 1);
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
} */

void test_sequence() {
    srand(time(NULL));
    heap h = createEmptyHeap();
    assert(initHeap(&h, 10, &compare, &setKey, &minKey) == 10);
    errorHandler();
    ticks programStart = now();
    ticks start;
    ticks end;
    Item* rm;

    Item* item0 = createItem(97);
    Item* item1 = createItem(82);
    Item* item2 = createItem(59);
    Item* item3 = createItem(99);
    Item* item4 = createItem(77);
    Item* item5 = createItem(34);
    Item* item6 = createItem(78);
    Item* item7 = createItem(46);
    Item* item8 = createItem(21);
    Item* item9 = createItem(45);

    heapInsert(&h, item0);
    heapInsert(&h, item1);
    heapInsert(&h, item2);
    heapInsert(&h, item3);
    heapInsert(&h, item4);
    heapInsert(&h, item5);

    assert(testHeapIntegrity(&h));

    printf("print 1\n");
    heapPrintTree(&h);

    assert(compare(_min(&h), item5) == 0);

    assert(decreaseKey(&h, item3, createItem(1)) != -1);
    errorHandler();

    // decrease key of item3 to 1 but still same element as item3
    Item* tmp = createItem(1);
    tmp->key = 1;
    tmp->element = item3->element;
    rm = extractMin(&h);
    assert(compare(rm, tmp) == 0);
    free(rm);


    rm = heapRemove(&h, item2);
    assert(compare(rm, item2) == 0);
    heapPrintTree(&h);
    assert(rm != NULL);
    free(rm);

    heapInsert(&h, item6);
    heapInsert(&h, item7);
    heapInsert(&h, item8);
    heapInsert(&h, item9);

    assert(testHeapIntegrity(&h));

    printf("print 2\n");
    heapPrintTree(&h);

    assert(compare(_min(&h), item8) == 0);
    rm = extractMin(&h);
    assert(compare(rm, item8) == 0);
    free(rm);
    rm = heapRemove(&h, item8);
    assert(rm == NULL);
    assert(errc == EH_DATA_DOESNT_EXIST);
    errcreset();
    assert(errc == SUCCESS);

    rm = extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item5) == 0);
    free(rm);
    
    rm = extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item9) == 0);
    free(rm);
    
    rm = extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item7) == 0);
    free(rm);
    
    rm = extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item4) == 0);
    free(rm);
    
    rm = extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item6) == 0);
    free(rm);
    
    rm = extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item1) == 0);
    free(rm);

    rm = extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item0) == 0);
    free(rm);

    freeHeap(&h);
    errorHandler();

    h = createEmptyHeap();
    assert(initHeap(&h, 1, &compare, &setKey, &minKey) == 1);
    errorHandler();

    item0 = createItem(97);
    item1 = createItem(82);
    item2 = createItem(59);
    item3 = createItem(99);
    item4 = createItem(77);
    item5 = createItem(34);
    item6 = createItem(78);
    item7 = createItem(46);
    item8 = createItem(21);
    item9 = createItem(45);

    Item* b[10] = {item0, item1, item2, item3, item4, item5, item6, item7, item8, item9};

    h = buildMinHeap((void*)&b, 10, &compare, &setKey, &minKey);

    printf("build min heap\n");
    heapPrintTree(&h);

    assert(testHeapIntegrity(&h));

    freeHeap(&h);
    errorHandler();
    printf("Tests passed.\n");
}

int main(void) {
    test_sequence();
    /* compute_1_to_n_sequences_of_operations(1000000, INSERTION);
    compute_1_to_n_sequences_of_operations(1000000, DELETION);
    return heap_integrity_test(300) ? 0 : -1; */
    return 0;
}