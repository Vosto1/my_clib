#!/bin/bash

if [ $0 == "" ]; then
    printf "usage: ./makeutils.sh [file]\n"
    printf "[file]: a file with all *.c file dependencies\n"
fi

# remove old ones if they are present
rm build.sh
rm memcheck.sh

# create new files
touch build.sh
touch memcheck.sh

# build.sh
TOP="#!/bin/bash\n# remove old executable\nrm prg.out\n# build\ngcc -Wall "
BOTTOM=" -o prg.out"
FILES=$(cat $0)

printf "$TOP" > build.sh
for $FILE in $FILES; do
    printf "$FILE " >> build.sh
done
printf "$BOTTOM" >> build.sh

# memcheck.sh
printf "#!/bin/bash\n# new file to save test results in\ntouch vmemtest.log\n# run memcheck tests\nvalgrind --log-file=\"vmemtest.log\" --leak-check=yes ./prg.out" > memcheck.sh

# exec privileges
chmod u+x build.sh
chmod u+x memcheck.sh