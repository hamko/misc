#!/bin/sh

make
valgrind --leak-check=full ./a.out  2> valgrind_output
