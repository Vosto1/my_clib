#!/bin/bash
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -g -o prg.out