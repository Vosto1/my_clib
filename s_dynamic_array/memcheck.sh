#!/bin/bash
# new file to save test results in
touch vmemtest.log
# run memcheck tests
valgrind --log-file="vmemtest.log" --leak-check=yes ./prg.out