#define _CRT_SECURE_NO_WARNINGS
#include "dynamic_array.h"
#include "../utils/error.h"
#include "assert.h"
#include "../utils/timer.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    int used;
    int size;
    double ratio;
}stats;

typedef struct {
    seconds s;
    int operation_amount;
    char operation[64];
}test_result;

int _max(int x, int y) {
    return x > y ? x : y;
}

double ratio(int used, int size) {
    return (double)used/(double)size;
}

void print_results(test_result res) {
    printf("computed %d %s operations during %f seconds.\n", res.operation_amount, res.operation, res.s);
    // write stats to file (in some good format, for desmos or wolfram for example)
}

void print_status(stats stat) {
    system("clear");
    printf("-------------------------\n");
    printf("-------array status------\n");
    printf("array size: %d\n", stat.size);
    printf("used indecies: %d\n", stat.used);
    printf("used vs size ratio: %f\n", stat.ratio);
    printf("-------------------------\n\n");
}

void insert_n(array * a, int n) {
    ErrorCode e;
    Item item;
    for (int j = 0; j < n; j++) {
        item.value = rand() % 1000;
        e = arrayInsert(a, item);
        errorHandler(e);
    }
    assert(a->used == n);
}

void remove_all(array* a) {
    ErrorCode e;
    for (int j = 0; a->used != 0; j++) {
        e = arrayRemoveLast(a);
        errorHandler(e);
    }
    assert(a->used == 0);
}

void auto_tests(int n, int mod) {
    array a = createEmptyDynamicArray();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    ErrorCode e = initDynamicArray(&a, 10);
    errorHandler(e);
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    Item item;
    srand(time(0));
    stats stat;
    test_result test_r;
    ticks start;
    ticks end;
    char operation[64];

    for (int i = 0; i < n; i++) {
        int next_tests = rand() % mod;
        int type = rand() % 5;
        
        switch (type) {
            case 0:
                sprintf(operation, "insert item");
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    item.value = rand() % 1000;
                    e = arrayInsert(&a, item);
                    errorHandler(e);
                }
                end = now();
                remove_all(&a);
            break;
            case 1:
                sprintf(operation, "remove item");
                insert_n(&a, next_tests);
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    e = arrayRemoveItem(&a, a.array[rand() % a.used]);
                    errorHandler(e);
                }
                end = now();
            break;
            case 2:
                sprintf(operation, "remove at");
                insert_n(&a, next_tests);
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    e = arrayRemoveAt(&a, rand() % a.used);
                    errorHandler(e);
                }
                end = now();
            break;
            case 3:
                sprintf(operation, "remove last");
                insert_n(&a, next_tests);
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    e = arrayRemoveLast(&a);
                    errorHandler(e);
                }
                end = now();
            break;
            case 4:
                sprintf(operation, "convert + union");
                start = now();
                for (int j = 0; j < next_tests; j++) {
                    int size = rand() % 100;
                    Data* b = (Data*)malloc(sizeof(Data)*size);
                    if (!b) {
                        printf("malloc error in test function.\n");
                        exit(-100);
                    }
                    for (int k = 0; k < size; k++)
                        b[k].value = rand() % 1000;
                    dynamicArray c = createEmptyDynamicArray();
                    e = initDynamicArray(&c, size);
                    errorHandler(e);
                    e = convert(b, &c, size);
                    errorHandler(e);
                    free(b);
                    e = arrayUnion(&a, &c);
                    errorHandler(e);
                }
                end = now();
                remove_all(&a);
            break;
        }
        end = now();
        seconds s = diff(start, end);
        strcpy(test_r.operation, operation);
        test_r.operation_amount = next_tests;
        test_r.s = s;
        print_results(test_r);
    }
    freeArray(&a);
    printf("Test passed.\n");
}

void test_sequence() {
    Item item;

    array a = createEmptyDynamicArray();
    assert(a.array == NULL);
    assert(a.size == 0);
    assert(a.used == 0);

    ErrorCode e = initDynamicArray(&a, 10);
    errorHandler(e);
    assert(a.array != NULL);
    assert(a.size == 10);
    assert(a.used == 0);

    for (int i = 0; i < 10; i++) {
        item.value = i;
        e = arrayInsert(&a, item);
        errorHandler(e);
        assert(a.used == (i + 1));
        assert(a.array[i].value == i);
    }

    printData(a.used, a.array);
    printf("%d\n\n", a.used);

    for (int i = 0; i < 5; i++) {
        item.value = (i + a.used);
        e = arrayInsert(&a, item);
        errorHandler(e);
    }
    assert(a.size == 20);
    assert(a.used == 15);

    printData(a.used, a.array);
    printf("%d\n\n", a.used);
    
    item.value = 1;
    arrayInsert(&a, item);

    item.value = 2;
    arrayInsert(&a, item);

    item.value = 3;
    arrayInsert(&a, item);

    item.value = 4;
    arrayInsert(&a, item);

    item.value = 5;
    arrayInsert(&a, item);
    for (int i = a.used; i > 5; i--) {
        arrayRemoveLast(&a);
        assert(a.used == (i - 1));
    }
    assert(a.size == 10);

    printData(a.used, a.array);
    printf("%d\n\n", a.used);

    item.value = 4;
    arrayRemoveItem(&a, item);

    item.value = 2;
    arrayRemoveItem(&a, item);
    assert(a.used == 3);
    
    printData(a.used, a.array);
    printf("%d\n\n", a.used);

    arrayRemoveAt(&a, 0);
    assert(a.size == 5);

    printData(a.used, a.array);
    printf("%d\n\n", a.used);

    freeArray(&a);
    printf("Tests passed.\n");
}

// dyn array test
int main(void) {
    //test_sequence();
    auto_tests(100, 1000000);
    return SUCCESS;
}