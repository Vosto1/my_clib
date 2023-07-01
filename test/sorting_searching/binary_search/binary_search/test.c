#include "test.h"

static int compare(const void* x, const void* y)
{
    int* x0 = (int*)x;
    int* y0 = (int*)y;

    if (*x0 < *y0)
    {
        return -1;
    }
    else if (*x0 > *y0)
    {
        return 1;
    }
    else// if (x0* == *y0)
    {
        return 0;
    }
}

static uint hashfn(const void* e, const hashtable* ht)
{
    int* f = (int*)e;
    
    int index = *f * 37;
    index %= ht_size(ht);
    if (index < 0)
    {
        index *= -1;
    }
    return index;
}

static int* new_num()
{
    int* tmp = (int*)malloc(sizeof(int));
    if (tmp == NULL)
    {
        perror("Malloc num error");
        exit(-1);
    }
    *tmp = rand() % 1000;
    return tmp;
}

void test(int iterations)
{
    // TODO: create tests for when the element is not present in the array
    srand(time(0));
    void (*free_num)(void*) = &free;
    mdarray array = mda_create_empty();
    uint size = 100;
    int* check = NULL;
    int* num = NULL;

    for (int i = 0; i < iterations; i++)
    {
        hashtable ht = ht_create_empty();
        assert(ht_init(&ht, size, &hashfn, &compare, free_num) == size);
        for (int j = 0; j < 100; j++)
        {
            do { // no duplicates in the array
                if (num != NULL)
                {
                    free(num);
                }
                num = new_num();
                check = (int*)ht_lookup(&ht, num); // use ht for fast lookup
            } while (check != NULL);
            ht_insert(&ht, num);
            num = NULL;
        }
        array = ht_to_array(&ht);
        ht_destroy(&ht);

        quicksort(array.array, array.used, &compare);
        int index = rand() % array.used; // get random value to search for
        printf("index: %d\n", index);

        int* value = (int*)mda_at(&array, index);
        printf("Value to search for: %d\n", *value);

        int location = binary_search(array.array, value, array.used, &compare);
        printf("Location in the array: %d, array[%d] = %d\n", location, location, *(int*)mda_at(&array, location));
        assert(*(int*)array.array[location] == *value);
        assert(index == location);


        mda_free(&array);
    }

    printf("Tests passed.\n");
}

