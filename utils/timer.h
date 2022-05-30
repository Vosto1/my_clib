#ifndef TIMER_H
#define TIMER_H

#include <time.h>

typedef double seconds;
typedef clock_t ticks;

/**
 * get cpu clock ticks
 * 
 * @return ticks since program start
 */
ticks now();

/**
 * get seconds passed between two cpu clock ticks measurements
 * 
 * @param start ticks at start time
 * @param end ticks at end time
 * @return seconds passed as a double
 */
seconds diff(ticks start, ticks end);

#endif
