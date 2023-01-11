#!/bin/bash

function rmbackups
{
    backups=$(find "$(pwd)" -regextype sed -regex ".*/.*\.h.bak$")
    for backup in $backups; do
        rm $backup
        printf "removed backup $(basename $backup) at $(dirname $backup)\n"
    done
}

function restore
{
    backups=$(find "$(pwd)" -regextype sed -regex ".*/.*\.h.bak$")
    for backup in $backups; do
        # remove .bak
        orig="${backup//'.bak'/""}"
        # revert
        mv $backup $orig
        printf "restored file $(basename $orig) at $(dirname $orig)\n"
    done
}

function recursive
{
    hfiles=$(find "$(pwd)" -regextype sed -regex ".*/.*\.h$")
}

function non_recursive
{
    hfilenames=$(ls | grep -e '\.h$')
    # add full path
    for hfilename in $hfilenames; do
        hfiles="$hfiles$(realpath $hfilename) "
    done
}

function run
{
    if [ -z "$hfiles" ]; then
        printf "the script must be used in a directory with header (.h) files!\n"
        exit -2
    fi
    guardregex='.*#ifndef.*#define.*#endif.*'
    for file in $hfiles; do
        # check if the file already has guards
        fguard=$(cat $file | tr -d '\n')
        if [[ "$fguard" =~ $guardregex ]]; then
            printf "the file "$file" already has guards\n"
        else
            # create backup
            bkp="$file.bak"
            contents=$(cp $file $bkp)
            printf "created backup for $(basename $file) at $(dirname $file)\n"
            # create guards
            name=$(basename $file)
            name="${name//".h"/"_H"}"
            # to uppercase
            name="${name^^}"
            printf '#ifndef %s\n#define %s\n\n\n#endif' $name $name > $file
            created="$created$file "
        fi
    done

    printf "\nguards created for:\n"
    for f in $created; do
        printf "$f\n"
    done
}

# PROGRAM START
if [ -n "$1" ] && [ $# -eq 1 ]; then
    case "$1" in
    "-r")
    # recursive create h-file guards (all dir below)
    recursive
    run
    exit 0
    ;;
    "-rb")
    # remove backups
    rmbackups
    exit 0
    ;;
    "-u")
    # restore using backups (undo)
    restore
    rmbackups
    exit 0
    ;;
    "--help")
    printf -- "\"-r\" : recursive create h-file guards (all dir below)\n\"-rb\": remove backups\n\"-u\" : undo and restore from backups\nno option: non-recursive create h-file guards (only current dir)\n"
    exit 0
    ;;
    *)
     # option not recognized
    printf "option not recognized ./createcguards --help for option info\n"
    exit -1
    ;;
    esac
# no arguments (run default)
elif [ -z "$1" ]; then
    # non-recursive create h-file guards
    non_recursive
    run
    exit 0
else
    # option not recognized
    printf "usage: ./createcguards [option]\n"
    exit -1
fi
