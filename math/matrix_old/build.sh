#!/bin/bash
# make it easier to read
clear
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -g ../../hash_dictionary/hash.c ../../hash_dictionary/hashtable.c list.c main.c matrix.c menu.c ../../s_dynamic_array/sdarray.c test2.c test.c ../../utils/counter.c ../../utils/timer.c ../../utils/error.c -lm  -o prg.out