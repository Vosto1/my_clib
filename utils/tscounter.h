#ifndef COUNTER_H
#define COUNTER_H
// thread safe

typedef unsigned int uint;

struct _cntr
{
    uint count;
};

typedef struct _cntr counter;

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

#endif