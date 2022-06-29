#!/bin/bash

# purpose: add c libraries to filesc.txt (cdeps)

canrun=$(ls | grep -e '^filesc.txt$')
if [ -z $canrun ]; then
    printf "you have to run cdepscan before running this script\n"
    exit -2
fi

# need atleast 1 argument
if [ $# -eq 0 ]; then
    printf "usage: ./addstr2cdeps [string1] [string2] ...\n"
    exit -1
fi

for arg in $@; do
    printf '%s\n' "$arg" >> filesc.txt
done

printf '%s added to filesc.txt\n' "$*"
exit 0