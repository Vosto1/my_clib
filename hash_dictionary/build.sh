#!/bin/bash
# remove old executable
rm prg.out
# build
gcc -Wall ../utils/error.c ../utils/timer.c ../s_dynamic_array/s_dynamic_array.c hashtable.c main.c test.c -o prg.out