#!/bin/bash
# make it easier to read
clear
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -g ../dynamic_string/dstring.c stringbuilder.c  -o prg.out