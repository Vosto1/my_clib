#!/bin/bash

# manually add a directory to a cdepscan file

canrun=$(ls | grep -e '^filesc.txt$')
if [ -z $canrun ]; then
    printf "you have to run cdepscan before running this script\n"
    exit -2
fi

if [ -z "$1" ]; then
    printf "usage: ./manualcomplcdeps [path]\n"
    exit -1
fi

path=$1

# get all .c files in the directory specified
cfiles=$(ls $1 | grep -e '\.c$')

# remove test.c
cfiles="${cfiles//'test.c'/""}"
# remove main.c
cfiles="${cfiles//'main.c'/""}"
# remove all spaces
cfiles="${cfiles//' '/""}"

for file in $cfiles; do
    printf "$path$file\n" >> filesc.txt
done

# exit normally
printf "filesc.txt updated\n"
exit 0