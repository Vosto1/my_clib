#!/bin/bash
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -g main.c priority_queue.c ../s_heap/s_heap.c test_priority_queue.c ../utils/error.c ../utils/timer.c ../dynamic_array/darray.c -lm -o prg.out
