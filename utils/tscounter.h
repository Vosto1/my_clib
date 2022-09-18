#ifndef COUNTER_H
#define COUNTER_H
// thread safe

#include "../datatype.h"

typedef struct _cntr counter;

struct _cntr
{
    uint count;
    void (*INC)(counter*);
    void (*RST)(counter*);
    uint (*CNT)(counter);
};

/**
 * @brief increment counter
 *
 */
void increment(counter *c);
/**
 * @brief reset counter
 *
 */
void reset(counter *c);
/**
 * @brief get counter value
 *
 */
uint count(counter c);
/**
 * @brief get a new initialized counter
 * 
 */
counter new_counter();

#endif