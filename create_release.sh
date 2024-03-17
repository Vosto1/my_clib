#!/bin/bash

clear

mkdir bin release release/include release/bin 2> /dev/null

printf "Building source files...\n"

# optimized release without debug info
#gcc -c -march=native -mtune=native -O3 ./src/*.c 2> /dev/null

# release with debug info
gcc -c  -g ./src/*.c 2> /dev/null
mv ./*.o ./bin/
ar -rcs ./bin/libutil.a ./bin/*.o
rm -f ./bin/*.o

printf "Creating archive of library files...\n"

cp ./src/*.h ./release/include/
cp ./bin/libutil.a ./release/bin/

tar --create --file ./release/release.tar ./release/include ./release/bin/*.a

printf "Cleaning up...\n"

rm -rf ./release/include ./release/bin

printf "Done\n"