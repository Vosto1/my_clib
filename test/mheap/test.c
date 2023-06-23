#include "test.h"

static int compare(const void *x, const void *y);
static void heapPrintTree(mheap *h);
static item *create_item(int x);
static void insert_n(mheap *h, int n);
static void remove_all(mheap *h);
static void freeObject(void* i);

// dynamic array
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

// mheap
static void heapPrintTree(mheap *h)
{
    int y = 0;
    int x = 0;
    for (int i = 0; i < mh_size(h); i++)
    {
        for (int j = 0; j < pow(2.0, (double)i) && j + pow(2.0, (double)i) <= mh_size(h); j++)
        {
            x = j + (int)pow(2.0, (double)i) - 1;
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

static void freeObject(void* i)
{
    free(i);
}

static void insert_n(mheap *h, int n)
{
    size_t e;
    item *it;
    for (int j = 0; j < n; j++)
    {
        it = create_item(rand() % 1000);
        e = mh_size(h) + 1;
        assert(mh_insert(h, (void *)it) == e);
    }
    assert(mh_size(h) == n);
}

static void remove_all(mheap *h)
{
    assert(mda_clear(&h->items) != 0);
    assert(mh_size(h) == 0);
}

void compute_1_to_n_sequences_of_operations(long n, Test type)
{
    mheap h = mh_create_empty();
    assert(mh_init(&h, 1, &compare, &freeObject) == 1);
    long j = 1;
    item *item;
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
                item = create_item(val);
                assert(mh_insert(&h, item) == mh_size(&h));
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
                item = mh_extract_min(&h);
                assert(item != NULL);
                free(item);
            }
            end = now();
            printf("Computed %ld extract min operations during %f seconds.\n", j, diff(start, end));
            j *= 2;
        }
        break;
    }
    assert(mh_free(&h));
}

bool heap_integrity_test(int n)
{
    mheap h = mh_create_empty();
    assert(mh_init(&h, 10, &compare, &freeObject) == 10);
    item *item;
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
                item = create_item(val);
                assert(mh_insert(&h, item) == mh_size(&h));
                // inc counter
                inscount++;
            }
            else
            { // 20%
                if (h.items.used != 0)
                {
                    item = mh_extract_min(&h);
                    assert(item != NULL);
                    free(item);
                    // inc counter
                    delcount++;
                }
            }
            if (!mh_test_heap_integrity(&h))
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
        printf("Computed %d operations and integrity checks (%d insertions and %d extract min) during %f seconds, integrity check passed.\n", x, inscount, delcount, diff(start, end));
        // reset counters
        delcount = 0;
        inscount = 0;
    }
    ticks testEnd = now();
    seconds passed = diff(testStart, testEnd);
    printf("Computed a total of %ld operations and tests.\n", totalTests);
    printf("Total test running time: %fs\n", passed);
    printf("Integrity test exiting...\n");
    assert(mh_free(&h));
    return true;
}

void test_sequence()
{
    srand(time(NULL));
    mheap h = mh_create_empty();
    assert(mh_init(&h, 10, &compare, &freeObject) == 10);
    item *rm;

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

    mh_insert(&h, item0);
    mh_insert(&h, item1);
    mh_insert(&h, item2);
    mh_insert(&h, item3);
    mh_insert(&h, item4);
    mh_insert(&h, item5);

    assert(mh_test_heap_integrity(&h));

    printf("print 1\n");
    heapPrintTree(&h);

    assert(compare(mh_peek(&h), item5) == 0);

    rm = mh_extract_min(&h);
    assert(compare(rm, item5) == 0);
    free(rm);

    mh_insert(&h, item6);
    mh_insert(&h, item7);
    mh_insert(&h, item8);
    mh_insert(&h, item9);

    assert(mh_test_heap_integrity(&h));

    printf("print 2\n");
    heapPrintTree(&h);

    assert(compare(mh_peek(&h), item8) == 0);
    rm = mh_extract_min(&h);
    assert(compare(rm, item8) == 0);
    free(rm);

    rm = mh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item9) == 0);
    free(rm);

    rm = mh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item7) == 0);
    free(rm);

    rm = mh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item2) == 0);
    free(rm);

    rm = mh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item4) == 0);
    free(rm);

    rm = mh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item6) == 0);
    free(rm);

    rm = mh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item1) == 0);
    free(rm);

    rm = mh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item0) == 0);
    free(rm);

    assert(mh_free(&h));

    h = mh_create_empty();
    assert(mh_init(&h, 1, &compare, &freeObject) == 1);

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

    h = mh_build_min_heap((void *)&b, 10, &compare, &freeObject);

    printf("build min mheap\n");
    heapPrintTree(&h);

    assert(mh_test_heap_integrity(&h));

    assert(mh_free(&h));
    printf("Tests passed.\n");
}
