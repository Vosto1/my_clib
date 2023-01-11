#!/bin/bash

# get files
# get all .h files in the directory
hfiles=$(ls | grep -e '\.h$')
# get all .c files in the directory
cfiles=$(ls | grep -e '\.c$')

# check if c/hfiles variables are empty
# if they are the script cannot run
if [ -z "$hfiles" ] || [ -z "$cfiles" ]; then
    printf "the script must be used in a directory with header (.h) and c source (.c) files!\n"
    exit -1
fi

# remove old file (if present)  and create new
rm filesc.txt &> /dev/null
touch filesc.txt

# find all included .h files in file
for file in $hfiles; do
    includes=$(cat $file | grep -e '^#include ".*\.h"$')
    # add to dependencies
    dependencies="$dependencies$includes"
done

# trim .h files
# remove "#include"
dependencies="${dependencies//'#include'/""}"
# remove quotes
dependencies="${dependencies//'"'/""}"
# replace ".h" with ".c"
dependencies="${dependencies//'.h'/".c"}"

# combine .h and .c files discovered in scan
dependencies="$dependencies$cfiles"
# remove all tabs, newlines, etc..
dependencies="${dependencies//[$'\t\r\n ']}"
# create word list
dependencies="${dependencies//".c"/".c "}"
# if *.c file doesn't exist, remove it (and the space after it) from dependencies
for dep in $dependencies; do
    if [ ! -f "$dep" ]; then
        dependencies="${dependencies//$dep /}"
    fi
done
# add newline at the end of each file name (instead of spaces)
dependencies="${dependencies//' '/"\\n"}"

dependencies=$(printf $dependencies | sort | uniq)
# merge included .c files and the .c files from this directory and write to file
printf "$dependencies\n" > filesc.txt
# exit normally
printf "directory scanned\n"
exit 0
