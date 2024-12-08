#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NOCOLOR='\033[0m'
clear

#remove existing compiled files
FILE1="a.out"
FILE2="main.o"

if [ -f "$FILE1" ] ; then
        rm "$FILE1"
fi
if [ -f "$FILE2" ] ; then
        rm "$FILE2"
fi


#compile with parameters used on progtest
g++ -std=c++17 -Wall -pedantic -Wno-long-long -O2 -Werror main.c

# get current directory and executable location
SCRIPT_DIR="$(pwd)"
PROG="${SCRIPT_DIR}/a.out"

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

# loop throught all input files in "sample/CZE/" directory
for IN_FILE in sample/CZE/*_in.txt; do

    #get reference files that are named as _out in ENG folder, the script will replace "_in" to "_out"
    REF_FILE=`echo $IN_FILE | sed -e 's/_in\(.*\)$/_out\1/'`

    #send input into the program and save the output to temp file
    $PROG < $IN_FILE > sample/script_check_temp.txt

    if ! diff -y $REF_FILE sample/script_check_temp.txt; then
        echo -e "${RED}-----------------FAIL: ${IN_FILE}------------------"
	echo -e "${NOCOLOR}"
        #exit
    else
        echo -e "${GREEN}---------------------SUCCESS-----------------------"
	echo -e "${NOCOLOR}"
    fi
done
