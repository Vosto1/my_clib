#!/bin/bash

# remove all scripts and testing files from the directory

# scripts
rm makeutils.sh &> /dev/null
rm cdepscan.sh &> /dev/null
rm memcheck.sh &> /dev/null
rm build.sh &> /dev/null

# files
rm vmemtest.log &> /dev/null
rm filesc.txt &> /dev/null
rm prg.out &> /dev/null

exit 0