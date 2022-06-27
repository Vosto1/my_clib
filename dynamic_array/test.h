#ifndef TEST_H
#define TEST_H

#define __TESTING__

#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include "darray.h"
#include "../utils/error.h"
#include "../utils/timer.h"

typedef struct
{
    int value;
} Item;

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
void auto_tests(int n, int mod);
void test_sequence();

#endif