#!/bin/bash

function getfiles_recursive
{
    FILES=$(find "$(pwd)" -regextype sed -regex $1)
}

function getfiles_non_recursive
{
    FILES=$(ls | grep -e $1)
}

function rmbackups
{
    backups=$FILES
    for backup in $backups; do
        rm $backup
        printf "removed backup $(basename $backup) at $(dirname $backup)\n"
    done
}

function restore_backups
{
    backups=$FILES
    for backup in $backups; do
        # remove .bak from the name
        orig="${backup//'.bak'/""}"
        # revert
        cp $backup $orig
        printf "restored file $(basename $orig) at $(dirname $orig)\n"
    done
}

function run
{
    hfiles=$FILES
    if [ -z "$hfiles" ]; then
        printf "the script must be used in a directory with header (.h) files or used recursive!\n"
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
    getfiles_recursive ".*/.*\.h$"
    run
    ;;
    "-rbr")
    # remove backups recursive
    getfiles_recursive ".*/.*\.h.bak$"
    rmbackups
    ;;
    "-rb")
    # remove backups
    getfiles_non_recursive "\.h.bak$"
    rmbackups
    ;;
    "-ur")
    # restore using backups recursive (undo)
    getfiles_recursive ".*/.*\.h.bak$"
    restore_backups
    rmbackups
    ;;
    "-u")
    # restore using backups (undo)
    getfiles_non_recursive "\.h.bak$"
    restore_backups
    rmbackups
    ;;
    "--help")
    printf -- "./createheaderguards [option]:\n\"-r\"  : recursive create h-file guards (all dir below)\n\"-rb\" : remove backups\n\"-rbr\": remove backups recursive\n\"-u\"  : undo and restore from backups\n\"-ur\" : undo and restore from backups recursive\nno option: non-recursive create h-file guards (only current dir)\n"
    exit 0
    ;;
    *)
     # option not recognized
    printf "option not recognized ./createheaderguards --help for option info\n"
    exit -1
    ;;
    esac
    exit 0
# no arguments (run default)
elif [ -z "$1" ]; then
    # non-recursive create h-file guards
    getfiles_non_recursive "\.h$"
    hfilenames=$FILES
    # add full path
    for hfilename in $hfilenames; do
        hfiles="$hfiles$(realpath $hfilename) "
    done
    run
    exit 0
else
    # option not recognized
    printf "usage: ./createheaderguards [option]\n"
    exit -1
fi
