#!/bin/bash

# builds library from files in ./lib

clear

mkdir bin release 2> /dev/null

gcc -c -Wall -Wextra ./lib/*.c
mv ./*.o ./bin/
ar -rcs ./bin/lib.a ./bin/*.o
rm -f ./bin/*.o