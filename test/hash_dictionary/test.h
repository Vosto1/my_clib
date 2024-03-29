#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <assert.h>
#include "hashtable.h"
#include "timer.h"

//#define HT_DEBUG
//#define HT_VERBOSE

typedef int value;
typedef char key;
typedef struct
{
    value v;
    key k;
} entry;

unsigned int auto_tests(int tests, int mod);
void test_sequence();

#endif