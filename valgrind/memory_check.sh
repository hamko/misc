#!/bin/sh

gcc -Wall -g main.c
valgrind --leak-check=full --show-reachable=yes ./a.out  2> valgrind_output
#valgrind -v ./a.out  2> valgrind_output
#valgrind -v --track-origins=yes ./a.out  2> valgrind_output
