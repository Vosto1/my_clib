#!/bin/bash

# dont move this script or cdepscan or makeutils!

if [ -z "$1"  ]; then
	printf "usage: ./copyto [path]\n"
	exit -1
fi

# create the dangerous script
#touch cleandir.sh
#printf '#!/bin/bash\n\n# remove all scripts and testing files from the directory\n\n# scripts\nrm makeutils.sh &> /dev/null\nrm cdepscan.sh &> /dev/null\nrm memcheck.sh &> /dev/null\nrm build.sh &> /dev/null\n\n# files\nrm vmemtest.log &> /dev/null\nrm filesc.txt &> /dev/null\nrm prg.out &> /dev/null\n\n# self destruct\nrm cleandir.sh &> /dev/null\n\nexit 0\n' > cleandir.sh


# === copy and arm cleandir ===
# ignore lines which begin with "# " or "#!"
# and uncomment lines which begins with only "#"
# (i.e. lines which dont match $ignore).
# #!/bin/bash is also removed,
# but added before reading the file instead
touch cleandir.sh
cldir="dangerous/cleandir.sh"
ignore='^#[ !]'
printf "#!/bin/bash\n" > cleandir.sh
while read -r line
do
	if [[ ! ${line} =~ ${ignore} ]]; then
		# uncomment line
		line=${line:1}
		# write to file
		printf "$line\n" >> cleandir.sh
	fi
	
done < "$cldir"
chmod u+x cleandir.sh


(cp "./cdepscan.sh" $1 && cp "./makeutils.sh" $1 && mv "./cleandir.sh" $1) || printf "copy error! The script shouldn't be moved from its original position\n" && exit -1

# exit normally
exit 0
