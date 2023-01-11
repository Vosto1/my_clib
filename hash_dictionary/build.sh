#!/bin/bash
# make it easier to read
clear
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -g hash.c hashtable.c main.c ../s_dynamic_array/sdarray.c test.c ../utils/timer.c  -o prg.out