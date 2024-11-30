#!/bin/bash
clear
FILE1="a.out"
FILE2="main.o"

if [ -f "$FILE1" ] ; then
	rm "$FILE1"
fi
if [ -f "$FILE2" ] ; then
	rm "$FILE2"
fi
g++ -std=c++17 -Wall -pedantic -Wno-long-long -g  main.c
