#include "test.h"

Item* createItem(int x) {
    Item* item = (Item*)malloc(sizeof(Item));
    item->element = (char)x;
    item->key = x;
    return item;
}

void insert_n(heap * h, int n) {
    size_t e;
    Item* item;
    for (int j = 0; j < n; j++) {
        item = createItem(rand() % 1000);
        e = s_heapSize(h) + 1;
        assert(s_heapInsert(h, (void*)item) == e);
        errorHandler();
    }
    assert(s_heapSize(h) == n);
}

void remove_all(heap* h) {
    assert(arrayClear(&h->items) != -1);
    assert(s_heapSize(h) == 0);
}

void compute_1_to_n_sequences_of_operations(long n, Test type) {
    heap h = s_createEmptyHeap();
    assert(s_initHeap(&h, 1, &compare) == 1);
    errorHandler();
    long j = 1;
    Item* item;
    ticks start;
    ticks end;
    switch (type) {
        case INSERTION:
            while (j <= n) {
                start = now();
                for (int i = 0; i < j; i++) {
                    int val = rand() % 1000;
                    item = createItem(val);
                    assert(s_heapInsert(&h, item) != -1);
                }
                end = now();
                printf("Computed %d insertion operations during %f seconds.\n", j, diff(start, end));
                j *= 2;
                remove_all(&h);
            }
        break;
        case DELETION:
            while (j <= n) {
                insert_n(&h, j);
                start = now();
                for (int i = 0; i < j; i++) {
                    item = s_extractMin(&h);
                    assert(item != NULL);
                    free(item);
                }
                end = now();
                printf("Computed %d extract min operations during %f seconds.\n", j, diff(start, end));
                j *= 2;
            }
        break;
    }
    s_freeHeap(&h);
    errorHandler();
}

bool heap_integrity_test(int n) {
    srand(time(NULL));
    heap h = s_createEmptyHeap();
    assert(s_initHeap(&h, 10, &compare) == 10);
    errorHandler();
    Item* item;
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
                assert(s_heapInsert(&h, item) != -1);
                // inc counter
                inscount++;
            }
            else
            { //20%
                if (h.items.used != 0) {
                    item = s_extractMin(&h);
                    assert(item != NULL);
                    free(item);
                    // inc counter
                    delcount++;
                } 
            }
            if(!s_testHeapIntegrity(&h)) {
                system("clear");
                printf("Heap integrity broken\n");
                heapPrintTree(&h);
                return false;
            }
            // inc test counter
            totalTests++;
        }
        end = now();
        printf("Computed %d operations (%d insertions and %d extract min) during %f seconds, passed integrity check.\n", x, inscount, delcount, diff(start, end));
        // reset counters
        delcount = 0;
        inscount = 0;
    }
    ticks testEnd = now();
    seconds passed = diff(testStart, testEnd);
    printf("Computed a total of %ld operations and tests.\n", totalTests);
    printf("Total test running time: %fs\n", passed);
    printf("Integrity test exiting...\n");
    s_freeHeap(&h);
    errorHandler();
    return true;
}

void test_sequence() {
    srand(time(NULL));
    heap h = s_createEmptyHeap();
    assert(s_initHeap(&h, 10, &compare) == 10);
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

    s_heapInsert(&h, item0);
    s_heapInsert(&h, item1);
    s_heapInsert(&h, item2);
    s_heapInsert(&h, item3);
    s_heapInsert(&h, item4);
    s_heapInsert(&h, item5);

    assert(s_testHeapIntegrity(&h));

    printf("print 1\n");
    heapPrintTree(&h);

    assert(compare(s_min(&h), item5) == 0);

    rm = s_extractMin(&h);
    assert(compare(rm, item5) == 0);
    free(rm);

    s_heapInsert(&h, item6);
    s_heapInsert(&h, item7);
    s_heapInsert(&h, item8);
    s_heapInsert(&h, item9);

    assert(s_testHeapIntegrity(&h));

    printf("print 2\n");
    heapPrintTree(&h);

    assert(compare(s_min(&h), item8) == 0);
    rm = s_extractMin(&h);
    assert(compare(rm, item8) == 0);
    free(rm);
    
    rm = s_extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item9) == 0);
    free(rm);
    
    rm = s_extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item7) == 0);
    free(rm);
    
    rm = s_extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item2) == 0);
    free(rm);

    rm = s_extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item4) == 0);
    free(rm);
    
    rm = s_extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item6) == 0);
    free(rm);
    
    rm = s_extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item1) == 0);
    free(rm);

    rm = s_extractMin(&h);
    assert(rm != NULL);
    assert(compare(rm, item0) == 0);
    free(rm);

    s_freeHeap(&h);
    errorHandler();

    h = s_createEmptyHeap();
    assert(s_initHeap(&h, 1, &compare) == 1);
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

    h = s_buildMinHeap((void*)&b, 10, &compare);

    printf("build min heap\n");
    heapPrintTree(&h);

    assert(s_testHeapIntegrity(&h));

    s_freeHeap(&h);
    errorHandler();
    printf("Tests passed.\n");
}

int main(void) {
    test_sequence();
    compute_1_to_n_sequences_of_operations(1000000, INSERTION);
    compute_1_to_n_sequences_of_operations(1000000, DELETION);
    return heap_integrity_test(300) ? 0 : -1;
    return 0;
}