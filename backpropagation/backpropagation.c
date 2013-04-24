#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nnsys.h"

void init_teacher_vect(nn_sys_t *nnins)
{
    float _ts[2], _tv[1];
    // 教師データの初期化
    // 第1教師
    _ts[0] = 0.0; _ts[1] = 0.0;
    _tv[0] = 0.0;
    nn_add_teacher_vect(nnins, _ts, _tv);
    // 第2教師
    _ts[0] = 0.0; _ts[1] = 1.0;
    _tv[0] = 1.0;
    nn_add_teacher_vect(nnins, _ts, _tv);
    // 第3教師
    _ts[0] = 1.0; _ts[1] = 0.0;
    _tv[0] = 1.0;
    nn_add_teacher_vect(nnins, _ts, _tv);
    // 第4教師
    _ts[0] = 1.0; _ts[1] = 1.0;
    _tv[0] = 0.0;
    nn_add_teacher_vect(nnins, _ts, _tv);
}

int main(void)
{
    int i;

    nn_sys_t nnins;
    // 実用の際には、第5引数は十分大きく取る方がよい。
    nn_init(&nnins, 2, 5, 1, 1, 0.1, 0.000001);
    init_teacher_vect(&nnins);

    for (i = 0; i < 4; i++) 
        printf("%f %f %f\n", getts(&nnins, i, 0), getts(&nnins, i, 1), gettv(&nnins, i, 0));

    while (1) {
        float e = 0.0;
        for (i = 0; i < nnins.teacher_n; i++) {
            nn_bp(&nnins, i);       // 教師ベクトル(理想の出力)との比較、重み係数調整
            e += nn_calcerr(&nnins, i);
        }

        fprintf(stderr, "e:%f\r", e);
        if (e < 0.001)
            break;
    }

    for (i = 0; i < 10000; i++) {
        float rx = (float)rand() / RAND_MAX * 3 - 1.5;
        float ry = (float)rand() / RAND_MAX * 3 - 1.5;
        float inputarr[2] = {rx, ry};
        nn_compute(&nnins, inputarr);  // 教師信号を入力し、実際の出力を出してみる
        printf("#XY %f %f %f %d\n", rx, ry, nnins.output[0], nnins.output[0] > 0.5 ? 1 : 0);
    }

    printf("\n");

    nn_free(&nnins);
    return 0;
}
