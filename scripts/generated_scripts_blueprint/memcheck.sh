#!/bin/bash
# make it easier to read
clear
# new file to save test results in
touch vmemtest.log
# run memcheck tests
if [ -z "$1" ]; then
    valgrind --log-file="vmemtest.log" --leak-check=yes ./prg.out
elif [ "$1" == "-p" ]; then
    valgrind --log-file="vmemtest.log" --leak-check=yes ./prg.out
    clear
    cat vmemtest.log
elif [ "$1" == "-pe" ]; then
    valgrind --show-error-list=yes --log-file="vmemtest.log" --leak-check=yes ./prg.out
    clear
    cat vmemtest.log
fi

exit 0