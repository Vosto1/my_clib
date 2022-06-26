#!/bin/bash

# make a string of a bash script and print to file

# if there was missing arguments
test -z "$1" || test -z "$2" && printf "usage: ./script2string [script] [file]\n" && exit -1
# if the file already exists
exists=$(ls | grep -e "^$2$")
test -n "$exists" && printf "cannot overwrite the file \"$2\"!\n" && exit -2
# if the script file doesnt exist
exists=$(ls | grep -e "^$1$")
test -z "$exists" && printf "the file \"$1\" doesnt exist!\n" && exit -3

# create file
touch $2
# read script line by line and write to file
printf "'" > $2
while read -r line
do
    printf "$line\\\\n" >> $2
done < "$1"
printf "'" >> $2

# exit normally
exit 0