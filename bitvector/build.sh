#!/bin/bash
# remove old executable
rm prg.out
# build
gcc -Wall bitvector.c main.c ../s_dynamic_array/sdarray.c test_bitvector.c ../utils/error.c ../utils/file.c ../utils/timer.c  -o prg.out