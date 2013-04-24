#!/bin/sh
make
./a.out > tmp
awk '$1 == "#XY" {$1 = ""; print}' tmp > xy

gnuplot <<eof
set terminal png
set output "bp.png"
splot "xy" u 1:2:3 w p
eof

#rm tmp one zero
