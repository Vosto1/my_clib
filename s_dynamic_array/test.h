#ifndef TEST_H
#define TEST_H

#define __TESTING__

#define _CRT_SECURE_NO_WARNINGS
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include "sdarray.h"
#include "../utils/error.h"
#include "../utils/timer.h"

typedef struct
{
    int value;
} item;

// userdefined function needed for the dyn-array
static int compare(const void *i1, const void *i2);

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
} testResult;

// test functions
static int _max(int x, int y);
static double ratio(int used, int size);
static void print_results(testResult res);
static void print_status(stats stat);
static item *create_item(int value);
static void printData(sdarray *a);
static void remove_all(sdarray *a);
void auto_tests(int n, int mod);
void test_sequence();

#endif