#!/bin/bash
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall ../dynamic_array/darray.c main.c s_heap.c test.c ../utils/error.c ../utils/timer.c -lm -o prg.out