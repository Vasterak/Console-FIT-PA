#!/bin/bash
rm a.out
rm main.o
g++ -std=c++17 -Wall -pedantic -Wno-long-long -O2 -c -Werror main.c
