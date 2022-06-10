#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include "btree.h"
#include "../utils/timer.h"
#include "../utils/error.h"

typedef char item;

void auto_tests(int n, int mod);

// test_sequence wont work if srand
// is called to initialize rand
void test_sequence();

#endif