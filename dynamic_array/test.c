#include "test.h"

static int compareItems(const void *i1, const void *i2)
{
    Item *item1 = (Item *)i1;
    Item *item2 = (Item *)i2;
    return item1->value - item2->value;
}

static int _max(int x, int y)
{
    return x > y ? x : y;
}

static double ratio(int used, int size)
{
    return (double)used / (double)size;
}

static void print_results(test_result res)
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

static Item *createItem(int value)
{
    Item *item = (Item *)malloc(sizeof(Item));
    item->value = value;
    return item;
}

static void printData(darray *a)
{
    Item *item;
    for (int i = 0; i < a->used; i++)
    {
        item = (Item *)a->array[i];
        printf("value at %d: %d\n", i, item->value);
    }
    printf("used: %d\nsize: %d\n\n", a->used, a->size);
}

static void insert_n(darray *a, int n)
{
    size_t e;
    Item *item;
    for (int j = 0; j < n; j++)
    {
        item = createItem(rand() % 1000);
        e = a->used + 1;
        assert(da_insert(a, (void *)item) == e);
        errorHandler();
    }
    assert(a->used == n);
}

static void remove_all(darray *a)
{
    da_clear(a);
    assert(a->used == 0);
}

void auto_tests(int n, int mod)
{
    darray a = da_create_empty();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    assert(da_init(&a, 10, &compareItems) == 10);
    errorHandler();
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    Item item;
    Item *d;
    srand(time(0));
    stats stat;
    test_result test_r;
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
                d = createItem(rand() % 1000);
                assert(da_insert(&a, (void *)d) != -1);
                errorHandler();
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
                d = (Item *)da_remove_item(&a, a.array[rand() % a.used]);
                assert(d != NULL);
                free(d);
                errorHandler();
            }
            end = now();
            break;
        case 2:
            sprintf(operation, "remove at");
            insert_n(&a, next_tests);
            start = now();
            for (int j = 0; j < next_tests; j++)
            {
                d = (Item *)da_remove_at(&a, rand() % a.used);
                assert(d != NULL);
                free(d);
                errorHandler();
            }
            end = now();
            break;
        case 3:
            sprintf(operation, "remove last");
            insert_n(&a, next_tests);
            start = now();
            for (int j = 0; j < next_tests; j++)
            {
                d = (Item *)da_remove_last(&a);
                assert(d != NULL);
                free(d);
                errorHandler();
            }
            end = now();
            break;
        case 4:
            start = now();
            unsigned long long operations = 0;
            for (int j = 0; j < next_tests; j++)
            {
                int size = rand() % 100 + 1;
                Item **b = (Item **)malloc(sizeof(Item *) * size);
                if (!b)
                {
                    printf("malloc error in test function.\n");
                    exit(-100);
                }
                for (int k = 0; k < size; k++)
                    b[k] = createItem(rand() % 1000);
                darray c = da_create_empty();
                assert(da_init(&c, size, &compareItems) == size);
                errorHandler();
                assert(da_convert(&c, (void *)b, size, &compareItems) == size);
                free(b);
                errorHandler();
                assert(da_merge(&a, &c) != -1);
                errorHandler();
                operations += size + size;
            }
            end = now();
            remove_all(&a);
            sprintf(operation, "convert + union (%lld insertions)", operations);
            break;
        }
        end = now();
        seconds s = diff(start, end);
        strcpy(test_r.operation, operation);
        test_r.operation_amount = next_tests;
        test_r.s = s;
        print_results(test_r);
    }
    da_free(&a);
    printf("Test passed.\n");
}

void test_sequence()
{
    Item item;
    Item *itemptr;

    darray a = da_create_empty();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    assert(da_init(&a, 10, &compareItems) != -1);
    errorHandler();
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    for (int i = 0; i < 10; i++)
    {
        itemptr = createItem(i);
        assert(da_insert(&a, (void *)itemptr) != -1);
        errorHandler();
        assert(a.used == (i + 1));
        itemptr = (Item *)a.array[i];
        assert(itemptr->value == i);
    }

    printData(&a);

    for (int i = 0; i < 5; i++)
    {
        itemptr = createItem(i + a.used);
        assert(da_insert(&a, (void *)itemptr) != -1);
        errorHandler();
        itemptr = (Item *)a.array[a.used - 1];
        assert(itemptr->value == i + (a.used - 1));
    }
    assert(a.size == 20);
    assert(a.used == 15);

    printData(&a);

    itemptr = createItem(1);
    assert(da_insert(&a, (void *)itemptr) != -1);

    itemptr = createItem(2);
    assert(da_insert(&a, (void *)itemptr) != -1);

    itemptr = createItem(3);
    assert(da_insert(&a, (void *)itemptr) != -1);

    itemptr = createItem(4);
    assert(da_insert(&a, (void *)itemptr) != -1);

    printData(&a);
    printf("%d %d\n\n", a.used, a.size);

    itemptr = createItem(5);
    assert(da_insert(&a, (void *)itemptr) != -1);
    for (int i = a.used; i > 5; i--)
    {
        itemptr = (Item *)da_remove_last(&a);
        assert(itemptr != NULL);
        free(itemptr);
        assert(a.used == (i - 1));
    }
    assert(a.size == 10);

    printData(&a);

    item.value = 4;
    itemptr = (Item *)da_remove_item(&a, (void *)&item);
    assert(itemptr != NULL);
    assert(itemptr->value == item.value);
    free(itemptr);

    item.value = 2;
    itemptr = (Item *)da_remove_item(&a, (void *)&item);
    assert(itemptr != NULL);
    assert(itemptr->value == item.value);
    free(itemptr);
    assert(a.used == 3);

    printData(&a);

    itemptr = (Item *)da_remove_at(&a, 0);
    assert(itemptr != NULL);
    assert(itemptr->value == 0);
    free(itemptr);
    assert(a.size == 5);

    printData(&a);

    assert(da_clear(&a) != -1);
    assert(a.used == 0);

    da_free(&a);
    assert(a.array == NULL);
    printf("Tests passed.\n");
}