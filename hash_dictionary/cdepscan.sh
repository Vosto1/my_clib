#!/bin/bash

# get files
# get all .h files in the directory
hfiles=$(ls | grep -e '\.h$')
# get all .c files in the directory
cfiles=$(ls | grep -e '\.c$')

# check if c/hfiles variables are empty
# if they are script cannot run
if [ -z "$hfiles" ] || [ -z "$cfiles" ]; then
    printf "script must be used in a directory with header (.h) and c source (.c) files!\n"
    exit -1
fi

# remove old file (if present)  and create new
rm filesc.txt &> /dev/null
touch filesc.txt


# replace space with newline
cfiles="${cfiles//' '/"\n"}"

# find all included .h files in file
for file in $hfiles; do
    includes=$(cat $file | grep -e '^#include ".*\.h"$')
    # remove "#include" and add a newline between files
    includes="${includes//\#include /""}"
    # add to deps variable one include at a time forcing it to become an array of strings
    for include in $includes; do
        dependencies="$dependencies\n$include"
    done
done

# remove quotes
dependencies="${dependencies//'"'/""}"
# replace .h with .c
dependencies="${dependencies//'.h'/".c"}"
# remove leading newline
dependencies="${dependencies/'\n'/""}"

# remove duplicates
rmduplicates=$(printf "$dependencies\n$cfiles\n" | sort | uniq)

# merge included .c files and the .c files from this directory and write to fil
printf "$rmduplicates" > filesc.txt
# exit normally
printf "directory scanned\n"
exit 0