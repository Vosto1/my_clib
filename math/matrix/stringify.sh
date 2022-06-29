#!/bin/bash

# stringify a file
# default is stringify each line in the file with double quotes

if [ -z "$1" ]; then
    printf "usage: ./stringify [file] [option ...]\n"
    exit -1
fi

# defaults:
ch="\""
opt="line" # quote each line

# get all arguments to the script except the first one
options="${@:2}"

# change defaults if supplied
for option in $options; do
    case $option in
    -c*)
    ch="${option:2}" # character was specified by the user
    ;;
    -a)
    opt="all" # quote the whole file (one long string)
    ;;
    *)
    printf '%s is not a recognized option\n' $option
    exit -2
    esac
done

# get file contents
file=$(cat $1)

newfilename="$1.stringify"
# rm old one if present
rm $newfilename &> /dev/null
# create new file
touch "$newfilename"

# stringify line by line
if [ $opt == "line" ]; then
    for line in $file; do
        printf '%s%s%s\n' "$ch" "$line" "$ch" >> $newfilename
    done
# stringify the whole file
elif [ $opt == "all" ]; then
    # replace all newlines with space
    file="${file//$'\n'/ }"
    printf '%s%s%s\n' "$ch" "$file" "$ch" > $newfilename
fi

printf 'stringified %s "%s" with "%s" wrote to %s\n' "$opt" "$1" "$ch" "$newfilename"
exit 0