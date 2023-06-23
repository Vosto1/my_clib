#!/bin/bash

# builds library from files in ./lib

clear

mkdir bin release 2> /dev/null

printf "Building...\n"

gcc -c -g -Wall -Wextra ./src/*.c # -g = debug flag
mv ./*.o ./bin/
ar -rcs ./bin/libutil.a ./bin/*.o

printf "Cleaning up...\n"

rm -f ./bin/*.o

printf "Done\n"