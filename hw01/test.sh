#!/bin/bash

clear

# get current directory and executable location
SCRIPT_DIR="$(pwd)"
PROG="${SCRIPT_DIR}/output.o"

# check if folder named "sample" exists in this directory, DOES NOT CHECK CONTENT OF THE FOLDER!
if ! test -d "${SCRIPT_DIR}/sample"; then

    #if folder does not exist extract the sample.tgz to new folder sample
    echo "Extracting the sample.tgz"
    mkdir sample
    tar -xzf "${SCRIPT_DIR}/sample.tgz" -C "${SCRIPT_DIR}/sample/"

    # check if extraction was successful
    if [ $? -eq 0 ]; then
        echo "Extraction was successful"
    else
        echo "Extraction failed"
        exit
    fi
fi

# loop throught all input files in "sample/ENG/" directory
for IN_FILE in sample/ENG/*_in.txt; do

    #get reference files that are named as _out in ENG folder, the script will replace "_in" to "_out"
    REF_FILE=`echo $IN_FILE | sed -e 's/_in\(.*\)$/_out\1/'`

    #send input into the program and save the output to temp file
    $PROG < $IN_FILE > sample/script_check_temp.txt

    if ! diff -qy $REF_FILE sample/script_check_temp.txt; then
        echo "Comparation failed with: ${IN_FILE}"
        exit
    else
        echo "Welldone!"
    fi
done