#!/bin/bash

# dont move this script or cdepscan or makeutils!

if [ -z "$1"  ]; then
	printf "usage: ./copyto [path]\n"
	exit -1
fi

(cp "./cdepscan.sh" $1 && cp "./makeutils.sh" $1 && cp "./dangerous/cleandir.sh" $1) || printf "copy error! The script shouldn't be moved from its original position\n" && exit -1

# exit normally
exit 0
