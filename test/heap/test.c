#include "test.h"

// user defined functions needed for the heap
static int compare(const void *x, const void *y);
static void *setKey(void *data, void *key);
static void minKey(void *base, void **out);
static void decrementKey(void *base, void **out);
static void freeObject(void* it);

// debug print
static void heapPrintTree(heap *h);

static int compare(const void *x, const void *y)
{
    item *item0 = (item *)x;
    item *item1 = (item *)y;
    if (item0->key > item1->key)
        return 1;
    else if (item0->key < item1->key)
        return -1;
    else
        return 0;
}


static void decrementKey(void *base, void **out)
{
    item *b = (item *)base;
    void *o = *out;
    item *oi = (item *)o;
    b->key -= 1.0f;   // decrement the smallest key in the heap
    oi->key = b->key; // set out key to new key
    b->key += 1.0f;   // rollback, because we are decrementing an actual item in the heap
}

// guarantees the smallest key unless the heap has been changed after
static void minKey(void *base, void **out)
{
    item *min = (item *)base;
    *out = (item *)malloc(sizeof(item));
    decrementKey(min, out);
}

static void *setKey(void *it, void *key)
{
    item *i = (item *)it;
    item *k = (item *)key;
    i->key = k->key;
    return i;
}

static void freeObject(void* it)
{
    free(it);
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
                item *it = (item *)h->items.array[x];
                printf("[k%f|%c]", it->key, it->element);
            }
            else
                printf("----");
        }
        printf("\n");
    }
}

static item *create_item(int x)
{
    item *it = (item *)malloc(sizeof(item));
    it->element = (char)x;
    it->key = x;
    return it;
}

static item *randomElement(heap *h)
{
    int index = rand() % h->items.used;
    return h->items.array[index];
}

static void insert_n(heap *h, int n)
{
    size_t e;
    item *item;
    for (int j = 0; j < n; j++)
    {
        item = create_item(rand() % 1000);
        e = h_size(h) + 1;
        assert(h_insert(h, (void *)item) == e);
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
    assert(h_init(&h, 1, &compare, &setKey, &minKey, &freeObject) == 1);
    long j = 1;
    item *it;
    item *toDelete;
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
                it = create_item(val);
                assert(h_insert(&h, it) == h_size(&h));
            }
            end = now();
            printf("Computed %ld insertion operations during %f seconds.\n", j, diff(start, end));
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
                it = h_remove(&h, toDelete);
                assert(it != NULL);
                free(it);
            }
            end = now();
            printf("Computed %ld deletion operations during %f seconds.\n", j, diff(start, end));
            j *= 2;
        }
        break;
    }
    assert(h_free(&h));
}

bool heap_integrity_test(int n)
{
    srand(time(NULL));
    heap h = h_create_empty();
    assert(h_init(&h, 10, &compare, &setKey, &minKey, &freeObject) == 10);
    item *it;
    item *toDelete;
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
                it = create_item(val);
                assert(h_insert(&h, it) == h_size(&h));
                // inc counter
                inscount++;
            }
            else
            { // 20%
                if (h.items.used != 0)
                {
                    toDelete = randomElement(&h);
                    it = h_remove(&h, toDelete);
                    assert(it != NULL);
                    free(it);
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
    assert(h_free(&h));
    return true;
}

void test_sequence()
{
    srand(time(NULL));
    heap h = h_create_empty();
    assert(h_init(&h, 10, &compare, &setKey, &minKey, &freeObject) == 10);
    item *rm;
    item *tmp;

    item *item0 = create_item(97);
    item *item1 = create_item(82);
    item *item2 = create_item(59);
    item *item3 = create_item(99);
    item *item4 = create_item(77);
    item *item5 = create_item(34);
    item *item6 = create_item(78);
    item *item7 = create_item(46);
    item *item8 = create_item(21);
    item *item9 = create_item(45);

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
    tmp = create_item(1);
    int i = h_decrease_key(&h, item3, tmp);
    free(tmp);
    assert(i != -1);
    assert(compare(h.items.array[i], item3) == 0);

    // decrease key of item3 to 1 but still same element as item3
    tmp = create_item(1);
    tmp->key = 1;
    tmp->element = item3->element;
    rm = h_extract_min(&h);
    assert(compare(rm, tmp) == 0);
    free(rm);
    free(tmp);

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
    item comp = *item8; // make stack copy
    free(rm);
    rm = h_remove(&h, &comp);
    assert(rm == NULL);

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

    assert(h_free(&h));
    
    h = h_create_empty();

    item0 = create_item(97);
    item1 = create_item(82);
    item2 = create_item(59);
    item3 = create_item(99);
    item4 = create_item(77);
    item5 = create_item(34);
    item6 = create_item(78);
    item7 = create_item(46);
    item8 = create_item(21);
    item9 = create_item(45);

    item *b[10] = {item0, item1, item2, item3, item4, item5, item6, item7, item8, item9};

    h = h_build_min_heap((void *)&b, 10, &compare, &setKey, &minKey, &freeObject);

    printf("build min heap\n");
    heapPrintTree(&h);

    assert(test_heap_integrity(&h));

    assert(h_free(&h));
    printf("Tests passed.\n");
}
