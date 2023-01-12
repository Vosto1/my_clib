#!/bin/bash

# remove all scripts and testing files from the directory

# I read the script into a string in the variable "cldir",
# and it is copied to and armed as a new file in the copyto.sh script.
# Arming: it removes all lines beginning with '# ' and uncomments all lines beginnning with '#',
# because this script is dangerous.

# scripts
#rm makeutils\.sh &> /dev/null
#rm cdepscan\.sh &> /dev/null
#rm memcheck\.sh &> /dev/null
#rm build\.sh &> /dev/null
#rm manualcomplcdeps\.sh &> /dev/null
#rm addstr2cdeps\.sh &> /dev/null
#rm rmstrcdeps\.sh &> /dev/null
#rm createheaderguards\.sh &> /dev/null
#rm implementheader\.sh &> /dev/null

# files
#rm vmemtest\.log &> /dev/null
#rm filesc\.txt &> /dev/null
#rm filesc\.txt.bak &> /dev/null
#rm prg\.out &> /dev/null
#rm core\.[0-9]* &> /dev/null
#rm vmemtest\.log\.core\.[0-9]* &> /dev/null

# self destruct
#rm cleandir\.sh &> /dev/null


#exit 0
