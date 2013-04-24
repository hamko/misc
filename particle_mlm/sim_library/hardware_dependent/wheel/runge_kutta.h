#ifndef RUNGE_KUTTA_H
#define RUNGE_KUTTA_H

void runge_kutta(double (*F)(double[], double, int, double[]), double, double[], double, int, double[]);
void add_vector(double[], double[], double[], int);

#endif
