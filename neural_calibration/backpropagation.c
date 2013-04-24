#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nnsys.h"
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define FL_ZERO(a) MAX(0.0, a)
#define TO_ONE(a) MIN(1.0, a)

float func(float x, float y) 
{
//    return (x + y) / 2;
    return (x * x + y * y) / 2;
}

float noise_func(float x, float y) 
{
    return FL_ZERO(TO_ONE(func(x, y) + 0.3 * rand() / RAND_MAX * (rand() > RAND_MAX / 2 ? 1 : 0)));
}

void init_teacher_vect(nn_sys_t *nnins)
{
    float _ts[2], _tv[1];
    // 教師データの初期化
    int i, j;
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) { 
            float x = (float)i / 10;
            float y = (float)j / 10;
            _ts[0] = x; _ts[1] = y;
            _tv[0] = noise_func(x, y);
            nn_add_teacher_vect(nnins, _ts, _tv);
        }
    }
}

int main(void)
{
    int i;
    int cyc;

    nn_sys_t nnins;
    nn_init(&nnins, 2, 7, 1, 100, 0.5, 0.00001);
    init_teacher_vect(&nnins);

    for (cyc = 0; cyc < 30000; cyc++) {
        float e = 0.0;
        for (i = 0; i < nnins.teacher_n; i++) {
            nn_bp(&nnins, i);
            e += nn_calcerr(&nnins, i);
        }

        fprintf(stderr, "e:%f\r", e);
    }

    for (i = 0; i < 10000; i++) {
        float rx = (float)rand() / RAND_MAX;
        float ry = (float)rand() / RAND_MAX;
        float inputarr[2] = {rx, ry};
        nn_compute(&nnins, inputarr);
        printf("#XY %f %f %f %f\n", rx, ry, nnins.output[0], nnins.output[0] - rx * rx / 2 - ry * ry / 2);
    }

    printf("\n");

    nn_free(&nnins);
    return 0;
}
