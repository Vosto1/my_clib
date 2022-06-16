#include "data.h"

// manual dynamic array
Data createPlaceholderData()
{
    Data item;
    item.value = PLACEHOLDER;
    return item;
}

resultType compareData(Data x, Data y)
{
    return x.value - y.value;
}

void printData(size_t size, Data a[])
{
    for (int i = 0; i < size; i++)
    {
        if (a[i].value == PLACEHOLDER)
            printf("element at index %d: PLACEHOLDER\n", i);
        else
            printf("element at index %d: %d\n", i, a[i].value);
    }
    printf("\n");
}