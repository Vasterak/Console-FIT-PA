#!/bin/bash
clear
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./a.out < inputTest.txt
