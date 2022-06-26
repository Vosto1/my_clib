#!/bin/bash
# remove old executable
rm prg.out &> /dev/null
# build
gcc -Wall -o prg.out