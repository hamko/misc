#include <stdio.h>
#include "rsys.h"

#define FUNC_NUM 2
#define MAX_SIMTIME 10000
#define DT 0.01

//val[0]: t
//val[1]: x
//val[2]: v
double f1(double *val)
{
    return val[2];
}

double f2(double *val)
{
    return -val[1];
}

int main(void)
{
    rungesys_t rsys;
    rsysfunc_t funcarr[FUNC_NUM];
    funcarr[0] = f1;
    funcarr[1] = f2;
    double initialarr[FUNC_NUM+1]; // 時間の分だけ一つ多い
    initialarr[0] = 0.0; // [s]
    initialarr[1] = 1.0; // [m]
    initialarr[2] = 0.0; // [m/s]
    rsys_init(&rsys, FUNC_NUM, funcarr, initialarr, DT);

    int i;
    for (i = 0; i < MAX_SIMTIME; i++) {
        double* p = rsys_get_state(&rsys);
        printf("%f %f %f\n", p[0], p[1], p[2]);
        rsys_revise(&rsys);
    }
}
