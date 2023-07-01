#include "test.h"

int* create_value(int v)
{
    int* tmp = (int*)malloc(sizeof(int));
    if (!tmp)
        exit(-2);
    *tmp = v;
    return tmp;
}

int compare(const void* i, const void* j)
{
    int *k = (int *)i;
    int *l = (int *)j;
    return *k - *l;
}

bool is_sorted(void* a[], size_t size)
{

    int* prev = (int*)a[0], *current;
    for (size_t i = 1; i < size; i++)
    {
        current = (int*)a[i];
        if (!(*current >= *prev))
            return false;
        prev = (int*)a[i];
    }
    return true;
}

void print(void* a[])
{
    int* j;
    for (int i = 0; i < SIZE; i++)
    {
        j = (int*)a[i];
        printf("%d ", *j);
    }
    printf("\n");
}

void init(void* a[], size_t size, int max)
{
    for (int i = 0; i < size; i++)
    {
        a[i] = create_value(rand() % max);
    }
}

void free_array(void* a[], size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        free(a[i]);
    }
    free(a);
}

void test(void)
{
    srand(time(0));
    //int arr[SIZE] = {33, 23, 13, 37, 44, 43, 48, 40, 34, 24, 45, 28, 21};
    void** a;
    ticks sinner, einner;
    seconds dinner;
    ticks start = now();
    for (int i = 0; i < 100; i++)
    {
        a = (void**)malloc(sizeof(void*) * SIZE);
        if (a == NULL)
            exit(-1);
        sinner = now();
        init(a, SIZE, 100);
        pcblue();
        printf("before:\n");
        pcreset();
        print(a);
        quicksort(a, SIZE, &compare);
        pcblue();
        printf("after:\n");
        pcreset();
        print(a);
        assert(is_sorted(a, SIZE));
        einner = now();
        dinner = diff(sinner, einner);
        pcblue();
        printf("Success");
        pcreset();
        printf(", time: %fs.\n", dinner);
        free_array(a, SIZE);
    }
    ticks end = now();
    seconds s = diff(start, end);
    pcgreen();
    printf("Tests passed");
    pcreset();
    printf(". %fs\n", s);
}