#!/bin/bash
# remove old executable
rm prg.out
# build
gcc -Wall main.c sdarray.c test.c ../utils/error.c ../utils/timer.c  -o prg.out