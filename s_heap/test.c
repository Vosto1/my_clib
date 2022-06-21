#include "test.h"

// dynamic array
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

// sheap
static void heapPrintTree(sheap *h)
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

static void insert_n(sheap *h, int n)
{
    size_t e;
    Item *item;
    for (int j = 0; j < n; j++)
    {
        item = createItem(rand() % 1000);
        e = sh_size(h) + 1;
        assert(sh_insert(h, (void *)item) == e);
        error_handler();
    }
    assert(sh_size(h) == n);
}

static void remove_all(sheap *h)
{
    assert(da_clear(&h->items) != 0);
    assert(sh_size(h) == 0);
}

void compute_1_to_n_sequences_of_operations(long n, Test type)
{
    sheap h = sh_create_empty();
    assert(sh_init(&h, 1, &compare) == 1);
    error_handler();
    long j = 1;
    Item *item;
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
                assert(sh_insert(&h, item) == sh_size(&h));
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
                item = sh_extract_min(&h);
                assert(item != NULL);
                free(item);
            }
            end = now();
            printf("Computed %d extract min operations during %f seconds.\n", j, diff(start, end));
            j *= 2;
        }
        break;
    }
    sh_free(&h);
    error_handler();
}

bool heap_integrity_test(int n)
{
    sheap h = sh_create_empty();
    assert(sh_init(&h, 10, &compare) == 10);
    error_handler();
    Item *item;
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
                assert(sh_insert(&h, item) == sh_size(&h));
                // inc counter
                inscount++;
            }
            else
            { // 20%
                if (h.items.used != 0)
                {
                    item = sh_extract_min(&h);
                    assert(item != NULL);
                    free(item);
                    // inc counter
                    delcount++;
                }
            }
            if (!s_test_heap_integrity(&h))
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
    sh_free(&h);
    error_handler();
    return true;
}

void test_sequence()
{
    srand(time(NULL));
    sheap h = sh_create_empty();
    assert(sh_init(&h, 10, &compare) == 10);
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

    sh_insert(&h, item0);
    sh_insert(&h, item1);
    sh_insert(&h, item2);
    sh_insert(&h, item3);
    sh_insert(&h, item4);
    sh_insert(&h, item5);

    assert(s_test_heap_integrity(&h));

    printf("print 1\n");
    heapPrintTree(&h);

    assert(compare(sh_peek(&h), item5) == 0);

    rm = sh_extract_min(&h);
    assert(compare(rm, item5) == 0);
    free(rm);

    sh_insert(&h, item6);
    sh_insert(&h, item7);
    sh_insert(&h, item8);
    sh_insert(&h, item9);

    assert(s_test_heap_integrity(&h));

    printf("print 2\n");
    heapPrintTree(&h);

    assert(compare(sh_peek(&h), item8) == 0);
    rm = sh_extract_min(&h);
    assert(compare(rm, item8) == 0);
    free(rm);

    rm = sh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item9) == 0);
    free(rm);

    rm = sh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item7) == 0);
    free(rm);

    rm = sh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item2) == 0);
    free(rm);

    rm = sh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item4) == 0);
    free(rm);

    rm = sh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item6) == 0);
    free(rm);

    rm = sh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item1) == 0);
    free(rm);

    rm = sh_extract_min(&h);
    assert(rm != NULL);
    assert(compare(rm, item0) == 0);
    free(rm);

    sh_free(&h);
    error_handler();

    h = sh_create_empty();
    assert(sh_init(&h, 1, &compare) == 1);
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

    h = sh_build_min_heap((void *)&b, 10, &compare);

    printf("build min sheap\n");
    heapPrintTree(&h);

    assert(s_test_heap_integrity(&h));

    sh_free(&h);
    error_handler();
    printf("Tests passed.\n");
}
