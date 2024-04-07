#!/bin/bash

BIN_INSTALL_DIR=/home/"$USER"/.clib/bin
INCLUDE_DIR=/home/"$USER"/.clib/include

mkdir -p "$BIN_INSTALL_DIR" "$INCLUDE_DIR"  2> /dev/null

cd ./release/ || exit $((-1))
tar -x -f release.tar

# move/overwrite
mv -f ./release/include/*.h "$INCLUDE_DIR/"
mv -f ./release/bin/*.a "$BIN_INSTALL_DIR/"
rm -rf ./release

printf "Installation successful:\n"
printf "    binaries in %s\n" "$BIN_INSTALL_DIR"
printf "    include files in %s\n" "$INCLUDE_DIR"