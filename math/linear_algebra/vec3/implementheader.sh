#!/bin/bash

# not a header file reg
hreg='!(\.h$)'

if [ -z "$1" ] || [[ "$1" =~ $hreg ]]; then
    printf "usage: ./implementheader [c header file]\n"
    exit -1
elif [ -z $(ls | grep -e "^$1$") ]; then
    printf "file doesnt exist in the directory $(pwd)\n"
    exit -2
fi

# create c file
cfile="${1/'.h'/'.c'}"
touch $cfile

# read file into array of strings
declare -a fnarray
declare i=0
read='.*);$'
while read -r line
do
	if [[ ${line} =~ ${read} ]]; then
        # add new element at the end of the array
		fnarray[$i]=$line
        ((i=$i+1))
	fi
done < "$1"

# include header in c file
printf "#include \"$1\"\n\n" > $cfile
# create function definitions
i=0
while [ $i -lt ${#fnarray[@]} ]; do
    #func="${fnarray[i]//';'/""}"
    #func="${func//'\n'/" "}"
    printf '%s\n' $fnarray[$i] >> $cfile
    printf "{\n\n}\n\n" >> $cfile
    ((i=$i+1))
done

printf "created fn defs in $cfile using header $1\n"
exit 0