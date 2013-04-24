#!/bin/sh
make
./a.out > tmp
awk '$1 == "#XY" && $5 == 0 {printf("%f %f\n", $2, $3)}' tmp > zero
awk '$1 == "#XY" && $5 == 1 {printf("%f %f\n", $2, $3)}' tmp > one

gnuplot <<eof
set terminal png
set output "/dev/null"
plot "one" u 1:2 w p
set output "bp.png"
replot "zero" u 1:2 w p
eof

#rm tmp one zero
