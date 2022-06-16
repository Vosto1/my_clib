#ifndef COUNTER_H
#define COUNTER_H

typedef unsigned int uint;

/**
 * @brief increment counter
 *
 */
void increment();
/**
 * @brief reset counter
 *
 */
void reset();
/**
 * @brief get counter value
 *
 */
uint count();

#define INC increment()
#define RST reset()
#define CNT count()

#endif