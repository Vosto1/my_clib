#!/bin/bash

mkdir -p /home/"$USER"/.clib/bin /home/"$USER"/.clib/include /home/"$USER"/.clib/bin 2> /dev/null

cd ./release/ || exit $((-1))
tar -x -f release.tar

# move/overwrite
mv -f ./release/include/*.h /home/"$USER"/.clib/include/
mv -f ./release/bin/*.a /home/"$USER"/.clib/bin/
rm -rf ./release