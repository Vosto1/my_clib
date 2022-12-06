#!/bin/bash
# make it easier to read
clear
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -g bstree.c main.c ../s_dynamic_array/sdarray.c test.c ../utils/error.c ../utils/timer.c -lm  -o prg.out