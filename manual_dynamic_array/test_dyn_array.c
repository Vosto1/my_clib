#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "m_dynamic_array.h"
#include "../utils/error.h"
#include "../utils/timer.h"

void test_sequence()
{
    errcinit();
    Item item;
    Item placeholder = createPlaceholderData();

    m_array a = m_createEmptyDynamicArray();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    assert(m_initDynamicArray(&a, 10) == 10);
    errorHandler();
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    printData(a.size, a.array);
    printf("used: %lld\n\n", a.used);

    for (int i = 0; i < 10; i++)
    {
        item.value = i;
        assert(m_arrayInsert(&a, item, i) == i);
        errorHandler();
        assert(a.used == (i + 1));
        assert(a.array[i].value == i);
    }

    printData(a.size, a.array);
    printf("used: %lld\n\n", a.used);

    for (int i = 0; i < 5; i++)
    {
        item.value = (i + a.used);
        assert(m_arrayInsert(&a, item, i) == -1);
        assert(errc == EARR_INDEX_OCCUPIED);
        errcset(SUCCESS);
        assert(a.array[i].value == i);
    }

    printData(a.size, a.array);
    printf("used: %lld\n\n", a.used);

    assert(m_checkMemoryIncrease(&a));
    m_memoryIncrease(&a);
    assert(a.size == 20);
    assert(a.used == 10);

    printData(a.size, a.array);
    printf("used: %lld\n\n", a.used);

    int j = a.used;
    for (int i = 0; i < 10; i++)
    {
        assert(m_arrayInsert(&a, item, j) == j);
        errorHandler();
        j++;
    }

    printData(a.size, a.array);
    printf("used: %lld\n\n", a.used);

    assert(a.used == 20);
    assert(a.size == 20);
    for (int i = a.used - 1; i > 4; i--)
    {
        assert(m_arrayRemoveAt(&a, i) == i);
    }
    assert(m_checkMemoryDecrease(&a));

    printData(a.size, a.array);
    printf("used: %lld\n\n", a.used);

    assert(m_memoryDecrease(&a) == MEM_HALVED);
    assert(a.size == 10);

    printData(a.size, a.array);
    printf("used: %lld\n\n", a.used);

    assert(m_arrayRemoveAt(&a, 2) == 2);
    assert(compareData(a.array[2], placeholder) == 0);

    printData(a.size, a.array);
    printf("used: %lld\n\n", a.used);

    printf("Test passed.\n");
    printf("The manual array might be useful for using with the hashtable.\n");
}

// manual dyn array test
int main(void)
{
    test_sequence();
    return 0;
}