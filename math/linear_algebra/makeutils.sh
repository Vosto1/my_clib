#!/bin/bash

if [ $# -ne 1 ]; then
    printf "usage: ./makeutils.sh [file]\n"
    exit -1
fi

# remove old ones if they are present
rm build.sh &> /dev/null
rm memcheck.sh &> /dev/null

# create new files
touch build.sh
touch memcheck.sh

# build.sh
TOP='#!/bin/bash\n# make it easier to read\nclear\n# remove old executable\nrm prg.out &> /dev/null\n# build\ngcc -Wall -g '
BOTTOM=' -o prg.out'
FILES=$(cat $1)

printf "$TOP" > build.sh
for FILE in $FILES; do
    printf '%s ' "$FILE" >> build.sh
done
printf "$BOTTOM" >> build.sh

# memcheck.sh
printf '#!/bin/bash\n# make it easier to read\nclear\n# new file to save test results in\ntouch vmemtest.log\n# run memcheck tests\nif [ -z "$1" ]; then\nvalgrind --log-file="vmemtest.log" --leak-check=yes ./prg.out\nelif [ "$1" == "-p" ]; then\nvalgrind --log-file="vmemtest.log" --leak-check=yes ./prg.out\nclear\ncat vmemtest.log\nelif [ "$1" == "-pe" ]; then\nvalgrind --show-error-list=yes --log-file="vmemtest.log" --leak-check=yes ./prg.out\nclear\ncat vmemtest.log\nfi\n\n' > memcheck.sh

# add exec privileges
chmod u+x build.sh
chmod u+x memcheck.sh

# exit normally
printf "build.sh and memcheck.sh created\n"
exit 0