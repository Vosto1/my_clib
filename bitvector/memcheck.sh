#!/bin/bash
# new file to save test results in
touch vmemtest.log
# run memcheck tests
if [ -z "$1" ]; then
    valgrind --log-file="vmemtest.log" --leak-check=yes ./prg.out
elif [ "$1" == "-s" ]; then
    printf "hello"
    valgrind --show-error-list=yes --log-file="vmemtest.log" --leak-check=yes ./prg.out
fi