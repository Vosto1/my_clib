#!/bin/bash

clear

gcc -c -Wall -Wextra ./lib/*.c
mv ./*.o ./bin/
ar -rcs ./bin/lib.a ./bin/*.o
rm -f ./bin/*.o

cp ./lib/*.h ./release/
cp ./bin/lib.a ./release/

tar --create --file ./release/release.tar ./release/*.h ./release/*.a

rm -f ./release/*.h ./release/*.a