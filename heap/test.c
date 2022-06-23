#include "test.h"

static int compare(const void *x, const void *y)
{
    Item *item0 = (Item *)x;
    Item *item1 = (Item *)y;
    if (item0->key > item1->key)
        return 1;
    else if (item0->key < item1->key)
        return -1;
    else
        return 0;
}

static void decrementKey(void *base, void **out)
{
    Item *b = (Item *)base;
    void *o = *out;
    Item *oi = (Item *)o;
    b->key -= 1.0f;   // decrement the smallest key in the heap
    oi->key = b->key; // set out key to new key
    b->key += 1.0f;   // rollback, because we are decrementing an actual item in the heap
}

// guarantees the smallest key unless the heap has been changed after
static void minKey(void *base, void **out)
{
    Item *min = (Item *)base;
    *out = (Item *)malloc(sizeof(Item));
    decrementKey(min, out);
}

static void *setKey(void *item, void *key)
{
    Item *i = (Item *)item;
    Item *k = (Item *)key;
    i->key = k->key;
    return i;
}

static void heapPrintTree(heap *h)
{
    int y = 0;
    int x = 0;
    for (int i = 0; i < h_size(h); i++)
    {
        for (int j = 0; j < pow(2, i) && j + pow(2, i) <= h_size(h); j++)
        {
            x = j + (int)pow(2, i) - 1;
            y = h->items.used;
            if (y > x)
            {
                Item *item = (Item *)h->items.array[x];
                printf("[k%f|%c]", item->key, item->element);
            }
            else
                printf("----");
        }
        printf("\n");
    }
}

static Item *createItem(int x)
{
    Item *item = (Item *)malloc(sizeof(Item));
    item->element = (char)x;
    item->key = x;
    return item;
}

static Item *randomElement(heap *h)
{
    int index = rand() % h->items.used;
    return h->items.array[index];
}

static void insert_n(heap *h, int n)
{
    dim_t e;
    Item *item;
    for (int j = 0; j < n; j++)
    {
        item = createItem(rand() % 1000);
        e = h_size(h) + 1;
        assert(h_insert(h, (void *)item) == e);
        error_handler();
    }
    assert(h_size(h) == n);
}

static void remove_all(heap *h)
{
    assert(da_clear(&h->items) != 0);
    assert(h_size(h) == 0);
}

void compute_1_to_n_sequences_of_operations(long n, Test type)
{
    heap h = h_create_empty();
    assert(h_init(&h, 1, &compare, &setKey, &minKey) == 1);
    error_handler();
    long j = 1;
    Item *item;
    Item *toDelete;
    ticks start;
    ticks end;
    switch (type)
    {
    case INSERTION:
        while (j <= n)
        {
            start = now();
            for (int i = 0; i < j; i++)
            {
                int val = rand() % 1000;
                item = createItem(val);
                assert(h_insert(&h, item) == h_size(&h));
            }
            end = now();
            printf("Computed %d insertion operations during %f seconds.\n", j, diff(start, end));
            j *= 2;
            remove_all(&h);
        }
        break;
    case DELETION:
        while (j <= n)
        {
            insert_n(&h, j);
            start = now();
            for (int i = 0; i < j; i++)
            {
                toDelete = randomElement(&h);
                item = h_remove(&h, toDelete);
                assert(item != NULL);
                free(item);
            }
            end = now();
            printf("Computed %d deletion operations during %f seconds.\n", j, diff(start, end));
            j *= 2;
        }
        break;
    }
    h_free(&h);
    error_handler();
}

bool heap_integrity_test(int n)
{
    srand(time(NULL));
    heap h = h_create_empty();
    assert(h_init(&h, 10, &compare, &setKey, &minKey) == 10);
    error_handler();
    Item *item;
    Item *toDelete;
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
            { // 80%
                item = createItem(val);
                assert(h_insert(&h, item) == h_size(&h));
                // inc counter
                inscount++;
            }
            else
            { // 20%
                if (h.items.used != 0)
                {
                    toDelete = randomElement(&h);
                    item = h_remove(&h, toDelete);
                    assert(item != NULL);
                    free(item);
                    // inc counter
                    delcount++;
                }
            }
            if (!test_heap_integrity(&h))
            {
                system("clear");
                printf("Heap integrity broken\n");
                heapPrintTree(&h);
                return false;
            }
            // inc test counter
            totalTests++;
        }
        end = now();
        printf("Computed %d operations and integrity checks (%d insertions and %d deletions) during %f seconds, integrity check passed.\n", x, inscount, delcount, diff(start, end));

        // reset counters
        delcount = 0;
        inscount = 0;
    }
    ticks testEnd = now();
    seconds passed = diff(testStart, testEnd);
    printf("Computed a total of %ld operations and tests.\n", totalTests);
    printf("Total test running time: %fs\n", passed);
    printf("Integrity test exiting...\n");
    h_free(&h);
    error_handler();
    return true;
}

void test_sequence()
{
    srand(time(NULL));
    heap h = h_create_empty();
    assert(h_init(&h, 10, &compare, &setKey, &minKey) == 10);
    error_handler();
    ticks programStart = now();
    ticks start;
    ticks end;
    Item *rm;

    Item *item0 = createItem(97);
    Item *item1 = createItem(82);
    Item *item2 = createItem(59);
    Item *item3 = createItem(99);
    Item *item4 = createItem(77);
    Item *item5 = createItem(34);
    Item *item6 = createItem(78);
    Item *item7 = createItem(46);
    Item *item8 = createItem(21);
    Item *item9 = createItem(45);

    h_insert(&h, item0);
    h_insert(&h, item1);
    h_insert(&h, item2);
    h_insert(&h, item3);
    h_insert(&h, item4);
    h_insert(&h, item5);

    assert(test_heap_integrity(&h));

    printf("print 1\n");
    heapPrintTree(&h);

    assert(compare(h_min(&h), item5) == 0);
    int i = h_decrease_key(&h, item3, createItem(1));
    assert(i != -1);
    assert(compare(h.items.array[i], item3) == 0);
    error_handler();

    // decrease key of item3 to 1 but still same element as item3
    Item *tmp = createItem(1);
    tmp->key = 1;
    tmp->element = item3->element;
    rm = h_extract_min(&h);
    assert(compare(rm, tmp) == 0);
    free(rm);

    rm = h_remove(&h, item2);
    assert(compare(rm, item2) == 0);
    heapPrintTree(&h);
    assert(rm != NULL);
    free(rm);

    h_insert(&h, item6);
    h_insert(&h, item7);
    h_insert(&h, item8);
    h_insert(&h, item9);

    assert(test_heap_integrity(&h));

    printf("print 2\n");
    heapPrintTree(&h);

    assert(compare(h_min(&h), item8) == 0);
    rm = h_extract_min(&h);
    assert(compare(rm, item8) == 0);
    free(rm);
    rm = h_remove(&h, item8);
    assert(rm == NULL);
    assert(errc == EH_DATA_DOESNT_EXIST);
    errcreset();
    assert(errc == SUCCESS);

    rm = h_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item5) == 0);
    free(rm);

    rm = h_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item9) == 0);
    free(rm);

    rm = h_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item7) == 0);
    free(rm);

    rm = h_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item4) == 0);
    free(rm);

    rm = h_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item6) == 0);
    free(rm);

    rm = h_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item1) == 0);
    free(rm);

    rm = h_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item0) == 0);
    free(rm);

    h_free(&h);
    error_handler();

    h = h_create_empty();
    assert(h_init(&h, 1, &compare, &setKey, &minKey) == 1);
    error_handler();

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

    Item *b[10] = {item0, item1, item2, item3, item4, item5, item6, item7, item8, item9};

    h = h_build_min_heap((void *)&b, 10, &compare, &setKey, &minKey);

    printf("build min heap\n");
    heapPrintTree(&h);

    assert(test_heap_integrity(&h));

    h_free(&h);
    error_handler();
    printf("Tests passed.\n");
}
