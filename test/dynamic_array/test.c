#include "test.h"

// userdefined function needed for the dyn-array
static int compare(const void *i1, const void *i2);

/*static int _max(int x, int y);
static double ratio(int used, int size);
static void print_status(stats stat);*/
static void print_results(testResult res);
static item *create_item(int value);
static void print_data(darray *a);
static void insert_n(darray *a, uint n);
static void remove_all(darray *a);
static void free_item(void* i);

static void free_item(void* i)
{
    free(i);
}

static int compare(const void *i1, const void *i2)
{
    item *item1 = (item *)i1;
    item *item2 = (item *)i2;
    return item1->value - item2->value;
}

/*static int _max(int x, int y)
{
    return x > y ? x : y;
}

static double ratio(int used, int size)
{
    return (double)used / (double)size;
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
}*/

static void print_results(testResult res)
{
    printf("computed %d %s operations during %f seconds.\n", res.operation_amount, res.operation, res.s);
    // write stats to file (in some good format, for desmos or wolfram for example)
}

static item *create_item(int value)
{
    item *i = (item *)malloc(sizeof(item));
    i->value = value;
    return i;
}

static void print_data(darray *a)
{
    item *it;
    for (int i = 0; i < a->used; i++)
    {
        it = (item *)a->array[i];
        printf("value at %d: %d\n", i, it->value);
    }
    printf("used: %u\nsize: %u\n\n", a->used, a->size);
}

static void insert_n(darray *a, uint n)
{
    uint e;
    item *item;
    for (uint j = 0; j < n; j++)
    {
        item = create_item(rand() % 1000);
        e = a->used + 1;
        assert(da_insert(a, (void *)item) == e);
    }
    assert(a->used == n);
}

static void remove_all(darray *a)
{
    assert(da_clear(a) != 0);
    assert(a->used == 0);
}

void auto_tests(int n, int mod)
{
    darray a = da_create_empty();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    assert(da_init(&a, 10, &compare, &free_item) == 10);
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    item *d;
    srand(time(0));
    testResult test_r;
    ticks start;
    ticks end;
    char operation[64];

    for (int i = 0; i < n; i++)
    {
        int next_tests = rand() % mod + 1;
        int type = rand() % 5;

        switch (type)
        {
        case 0:
            sprintf(operation, "insert item");
            start = now();
            for (int j = 0; j < next_tests; j++)
            {
                d = create_item(rand() % 1000);
                assert(da_insert(&a, (void *)d) != 0);
            }
            end = now();
            remove_all(&a);
            break;
        case 1:
            sprintf(operation, "remove item");
            insert_n(&a, next_tests);
            start = now();
            for (int j = 0; j < next_tests; j++)
            {
                d = (item *)da_remove_item(&a, a.array[rand() % a.used]);
                assert(d != NULL);
                free(d);
            }
            end = now();
            break;
        case 2:
            sprintf(operation, "remove at");
            insert_n(&a, next_tests);
            start = now();
            for (int j = 0; j < next_tests; j++)
            {
                d = (item *)da_remove_at(&a, rand() % a.used);
                assert(d != NULL);
                free(d);
            }
            end = now();
            break;
        case 3:
            sprintf(operation, "remove last");
            insert_n(&a, next_tests);
            start = now();
            for (int j = 0; j < next_tests; j++)
            {
                d = (item *)da_remove_last(&a);
                assert(d != NULL);
                free(d);
            }
            end = now();
            break;
        case 4:
            start = now();
            unsigned long long operations = 0;
            for (int j = 0; j < next_tests; j++)
            {
                int size = rand() % 100 + 1;
                darray c = da_create_empty();
                assert(da_init(&c, size, &compare, &free_item) == size);
                for (int k = 0; k < size; k++)
                    da_insert(&c, create_item(rand() % 1000));
                for (int k = 0; k < size; k++)
                    da_insert(&a, create_item(rand() % 1000));
                assert(da_merge(&a, &c) != 0);
                operations += size + size;
            }
            end = now();
            assert(da_clear(&a) != 0);
            sprintf(operation, "merge (%lld insertions)", operations);

            // extra tests
            darray b = da_create_empty();
            assert(da_init(&b, 10, &compare, &free_item) == 10);
            for (int i = 0; i < rand() % 20; i++)
                da_insert(&b, create_item(rand() % 100));
            assert(da_clear(&b) != 0);
            assert(da_destroy(&b));
            // clear followed by destroy is the same as free
            break;
        }
        seconds s = diff(start, end);
        strcpy(test_r.operation, operation);
        test_r.operation_amount = next_tests;
        test_r.s = s;
        print_results(test_r);
    }
    assert(da_free(&a));
    printf("Test passed.\n");
}

void test_sequence()
{
    item it;
    item *itemptr;

    darray a = da_create_empty();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    assert(da_init(&a, 10, &compare, &free_item) != 0);
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    for (int i = 0; i < 10; i++)
    {
        itemptr = create_item(i);
        assert(da_insert(&a, (void *)itemptr) != 0);
        assert(a.used == (i + 1));
        itemptr = (item *)a.array[i];
        assert(itemptr->value == i);
    }

    print_data(&a);

    for (int i = 0; i < 5; i++)
    {
        itemptr = create_item(i + a.used);
        assert(da_insert(&a, (void *)itemptr) != 0);
        itemptr = (item *)a.array[a.used - 1];
        assert(itemptr->value == i + (a.used - 1));
    }
    assert(a.size == 20);
    assert(a.used == 15);

    print_data(&a);

    itemptr = create_item(1);
    assert(da_insert(&a, (void *)itemptr) != 0);

    itemptr = create_item(2);
    assert(da_insert(&a, (void *)itemptr) != 0);

    itemptr = create_item(3);
    assert(da_insert(&a, (void *)itemptr) != 0);

    itemptr = create_item(4);
    assert(da_insert(&a, (void *)itemptr) != 0);

    print_data(&a);

    itemptr = create_item(5);
    assert(da_insert(&a, (void *)itemptr) != 0);
    for (int i = a.used; i > 5; i--)
    {
        itemptr = (item *)da_remove_last(&a);
        assert(itemptr != NULL);
        free(itemptr);
        assert(a.used == (i - 1));
    }
    assert(a.size == 10);

    print_data(&a);

    it.value = 4;
    itemptr = (item *)da_remove_item(&a, (void *)&it);
    assert(itemptr != NULL);
    assert(itemptr->value == it.value);
    free(itemptr);

    it.value = 2;
    itemptr = (item *)da_remove_item(&a, (void *)&it);
    assert(itemptr != NULL);
    assert(itemptr->value == it.value);
    free(itemptr);
    assert(a.used == 3);

    print_data(&a);

    itemptr = (item *)da_remove_at(&a, 0);
    assert(itemptr != NULL);
    assert(itemptr->value == 0);
    free(itemptr);
    assert(a.size == 5);

    print_data(&a);

    assert(da_clear(&a) != 0);
    assert(a.used == 0);

    assert(da_free(&a));
    assert(a.array == NULL);
    printf("Tests passed.\n");
}