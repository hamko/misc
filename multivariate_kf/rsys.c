#include <stdio.h>
#include <stdlib.h>
#include "rsys.h"

int rsys_init(rungesys_t* rsys, int dim, rsysfunc_t* funcarr, double* initarr, double dt)
{
    int i;

    rsys->funcarr = malloc(sizeof(rsysfunc_t) * dim);
    rsys->state = malloc(sizeof(double) * (dim+1));
    for (i = 0; i < dim; i++)
        rsys->funcarr[i] = funcarr[i];
    for (i = 0; i < dim+1; i++)
        rsys->state[i] = initarr[i];
    rsys->dim = dim;
    rsys->dt = dt;

    return 0;
}

int rsys_revise(rungesys_t* rsys)
{
    int i;
    double* k1 = malloc(rsys->dim * sizeof(double));
    double* k2 = malloc(rsys->dim * sizeof(double));
    double* k3 = malloc(rsys->dim * sizeof(double));
    double* k4 = malloc(rsys->dim * sizeof(double));
    
    for (i = 0; i < rsys->dim; i++) {
        k1[i] = rsys->dt * rsys->funcarr[i](rsys->state);
    }

    double* in2 = malloc((rsys->dim+1) * sizeof(double));
    for (i = 0; i < rsys->dim; i++) {
        in2[0] = rsys->state[0] + rsys->dt/2.0;
        int j;
        for (j = 0; j < rsys->dim; j++) {
            in2[j+1] = rsys->state[j+1] + k1[j] / 2.0;
        }

        k2[i] = rsys->dt * rsys->funcarr[i](in2);
    }
    free(in2);

    double* in3 = malloc((rsys->dim+1) * sizeof(double));
    for (i = 0; i < rsys->dim; i++) {
        in3[0] = rsys->state[0] + rsys->dt/2.0;
        int j;
        for (j = 0; j < rsys->dim; j++) {
            in3[j+1] = rsys->state[j+1] + k2[j] / 2.0;
        }

        k3[i] = rsys->dt * rsys->funcarr[i](in3);
    }
    free(in3);

    double* in4 = malloc((rsys->dim+1) * sizeof(double));
    for (i = 0; i < rsys->dim; i++) {
        in4[0] = rsys->state[0] + rsys->dt;
        int j;
        for (j = 0; j < rsys->dim; j++) {
            in4[j+1] = rsys->state[j+1] + k3[j];
        }

        k4[i] = rsys->dt * rsys->funcarr[i](in4);
    }
    free(in4);

    rsys->state[0] += rsys->dt;
    for (i = 0; i < rsys->dim; i++)
        rsys->state[i+1] += (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]) / 6.0;

    free(k1);
    free(k2);
    free(k3);
    free(k4);
}

double* rsys_get_state(rungesys_t* rsys)
{
    return rsys->state;
}
