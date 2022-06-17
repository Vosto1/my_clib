#ifndef TEST_H
#define TEST_H

#define __TESTING__

#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include "dynamic_array.h"
#include "../utils/error.h"
#include "../utils/timer.h"

typedef struct
{
    int value;
} Item;

// userdefined function needed for the dyn-array
static int compareItems(const void *i1, const void *i2);

typedef struct
{
    int used;
    int size;
    double ratio;
} stats;

typedef struct
{
    seconds s;
    int operation_amount;
    char operation[64];
} test_result;

// test functions
static int _max(int x, int y);
static double ratio(int used, int size);
static void print_results(test_result res);
static void print_status(stats stat);
static Item *createItem(int value);
static void printData(darray *a);
static void remove_all(darray *a);
void auto_tests(int n, int mod);
void test_sequence();

#endif