#!/bin/bash

clear

mkdir bin release release/include 2> /dev/null

gcc -c -Wall -Wextra ./src/*.c
mv ./*.o ./bin/
ar -rcs ./bin/libutil.a ./bin/*.o
rm -f ./bin/*.o

cp ./src/*.h ./release/include/
cp ./bin/libutil.a ./release/

tar --create --file ./release/release.tar ./release/include ./release/*.a

rm -rf ./release/include ./release/*.a