#include "test.h"

static int compare(const void *i1, const void *i2)
{
    item *item1 = (item *)i1;
    item *item2 = (item *)i2;
    return item1->value - item2->value;
}

static void freeObject(voidp_t i)
{
    free(i);
}

static int _max(int x, int y)
{
    return x > y ? x : y;
}

static double ratio(int used, int size)
{
    return (double)used / (double)size;
}

static void print_results(testResult res)
{
    printf("computed %d %s operations during %f seconds.\n", res.operation_amount, res.operation, res.s);
    // write stats to file (in some good format, for desmos or wolfram for example)
}

static void print_status(stats stat)
{
    system("clear");
    printf("-------------------------\n");
    printf("-------array status------\n");
    printf("array size: %d\n", stat.size);
    printf("used indecies: %d\n", stat.used);
    printf("used vs size ratio: %f\n", stat.ratio);
    printf("-------------------------\n\n");
}

static item *create_item(int value)
{
    item *it = (item *)malloc(sizeof(item));
    it->value = value;
    return it;
}

static void printData(sdarray *a)
{
    item *it;
    for (int i = 0; i < a->used; i++)
    {
        it = (item *)a->array[i];
        printf("value at %d: %d\n", i, it->value);
    }
    printf("used: %ld\nsize: %ld\n\n", a->used, a->size);
}

static void insert_n(sdarray *a, int n)
{
    size_t e;
    item *item;
    for (int j = 0; j < n; j++)
    {
        item = create_item(rand() % 1000);
        e = a->used + 1;
        assert(sda_insert(a, (void *)item) == e);
    }
    assert(a->used == n);
}

static void remove_all(sdarray *a)
{
    sda_clear(a);
    assert(a->used == 0);
}

void auto_tests(int n, int mod)
{
    sdarray a = sda_create_empty();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    assert(sda_init(&a, 10, &freeObject) == 10);
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    item it;
    item *d;
    srand(time(0));
    stats stat;
    testResult test_r;
    ticks start;
    ticks end;
    char operation[64];

    for (int i = 0; i < n; i++)
    {
        int next_tests = rand() % mod + 1;
        int type = rand() % 4;

        switch (type)
        {
        case 0:
            sprintf(operation, "insert item");
            start = now();
            for (int j = 0; j < next_tests; j++)
            {
                d = create_item(rand() % 1000);
                assert(sda_insert(&a, (void *)d) != 0);
            }
            end = now();
            remove_all(&a);
            break;
        case 1:
            sprintf(operation, "remove at");
            insert_n(&a, next_tests);
            start = now();
            for (int j = 0; j < next_tests; j++)
            {
                d = (item *)sda_remove_at(&a, rand() % a.used);
                assert(d != NULL);
                free(d);
            }
            end = now();
            break;
        case 2:
            sprintf(operation, "remove last");
            insert_n(&a, next_tests);
            start = now();
            for (int j = 0; j < next_tests; j++)
            {
                d = (item *)sda_remove_last(&a);
                assert(d != NULL);
                free(d);
            }
            end = now();
            break;
        case 3: // merge + some extra tests
            start = now();
            unsigned long long operations = 0;
            for (int j = 0; j < next_tests; j++)
            {
                int size = rand() % 100 + 1;
                sdarray c = sda_create_empty();
                assert(sda_init(&c, size, &freeObject) == size);
                for (int k = 0; k < size; k++)
                    sda_insert(&c, create_item(rand() % 1000));
                for (int k = 0; k < size; k++)
                    sda_insert(&a, create_item(rand() % 1000));
                assert(sda_merge(&a, &c) != 0);
                operations += size + size;
            }
            end = now();
            sda_clear(&a);
            sprintf(operation, "merge (%lld insertions)", operations);
            // extra tests
            sdarray b = sda_create_empty();
            assert(sda_init(&b, 10, &freeObject) == 10);
            for (int i = 0; i < rand() % 20; i++)
                sda_insert(&b, create_item(rand() % 100));
            sda_clear(&b);
            assert(sda_destroy(&b));
            // clear followed by destroy is the same as free
            break;
        }
        seconds s = diff(start, end);
        strcpy(test_r.operation, operation);
        test_r.operation_amount = next_tests;
        test_r.s = s;
        print_results(test_r);
    }
    assert(sda_free(&a));
    printf("Test passed.\n");
}

void test_sequence()
{
    item it;
    item *itemptr;

    sdarray a = sda_create_empty();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    assert(sda_init(&a, 10, &freeObject) == 10);
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    for (int i = 0; i < 10; i++)
    {
        itemptr = create_item(i);
        assert(sda_insert(&a, (void *)itemptr) != 0);
        assert(a.used == (i + 1));
        itemptr = (item *)a.array[i];
        assert(itemptr->value == i);
    }

    printData(&a);

    for (int i = 0; i < 5; i++)
    {
        itemptr = create_item(i + a.used);
        assert(sda_insert(&a, (void *)itemptr) != 0);
        itemptr = (item *)a.array[a.used - 1];
        assert(itemptr->value == i + (a.used - 1));
    }
    assert(a.size == 20);
    assert(a.used == 15);

    printData(&a);

    itemptr = create_item(1);
    assert(sda_insert(&a, (void *)itemptr) != 0);

    itemptr = create_item(2);
    assert(sda_insert(&a, (void *)itemptr) != 0);

    itemptr = create_item(3);
    assert(sda_insert(&a, (void *)itemptr) != 0);

    itemptr = create_item(4);
    assert(sda_insert(&a, (void *)itemptr) != 0);

    printData(&a);
    printf("%ld %ld\n\n", a.used, a.size);

    itemptr = create_item(5);
    assert(sda_insert(&a, (void *)itemptr) != 0);
    for (int i = a.used; i > 5; i--)
    {
        itemptr = (item *)sda_remove_last(&a);
        assert(itemptr != NULL);
        free(itemptr);
        assert(a.used == (i - 1));
    }
    assert(a.size == 10);

    printData(&a);

    itemptr = (item *)sda_remove_at(&a, 4);
    assert(itemptr != NULL);
    assert(itemptr->value == 4);
    free(itemptr);

    itemptr = (item *)sda_remove_at(&a, 2);
    assert(itemptr != NULL);
    assert(itemptr->value == 2);
    free(itemptr);
    assert(a.used == 3);

    itemptr = (item *)sda_remove_at(&a, 0);
    assert(itemptr != NULL);
    assert(itemptr->value == 0);
    free(itemptr);
    assert(a.size == 5);

    printData(&a);

    assert(sda_clear(&a) != 0);
    assert(a.used == 0);

    assert(sda_free(&a));
    assert(a.array == NULL);
    printf("Tests passed.\n");
}
