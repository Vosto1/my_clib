#!/bin/bash

# builds library from files in ./lib

clear

mkdir bin release 2> /dev/null

gcc -c -Wall -Wextra ./src/*.c
mv ./*.o ./bin/
ar -rcs ./bin/libutil.a ./bin/*.o
rm -f ./bin/*.o