#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "nnsys.h"

// teacher番目の入力ベクトルの、i番目のエレメントにvalを代入
void setts(nn_sys_t *nnins, int teacher, int i, float val)
{
    nnins->ts[teacher*nnins->input_n+i] = val;
}

// teacher番目の入力ベクトルの、i番目のエレメントを返す
float getts(nn_sys_t *nnins, int teacher, int i)
{
    return nnins->ts[teacher*nnins->input_n+i];
}

// teacher番目の入力ベクトルの、i番目のエレメントにvalを代入
void settv(nn_sys_t *nnins, int teacher, int i, float val)
{
    nnins->tv[teacher*nnins->output_n+i] = val;
}

// teacher番目の入力ベクトルの、i番目のエレメントを返す
float gettv(nn_sys_t *nnins, int teacher, int i)
{
    return nnins->tv[teacher*nnins->output_n+i];
}


// nn_initのあとに呼び出す
// _tsと_tvをnnins->ts, tvに代入し、teacher_nを1増やす。
// ts, tvが足りなくなったらreallocする。
void nn_add_teacher_vect(nn_sys_t *nnins, float *_ts, float *_tv)
{
    int i;
    int n = (nnins->teacher_n++);
    if (n > nnins->teacher_n_max - 1) {
        nnins->teacher_n_max *= 2;
        nnins->ts = (float*)realloc(nnins->ts, nnins->teacher_n_max*nnins->input_n*sizeof(float));
        nnins->tv = (float*)realloc(nnins->tv, nnins->teacher_n_max*nnins->output_n*sizeof(float));
    }

    for (i = 0; i < nnins->input_n; i++) {
        printf("ts: %d %d %f\n", n, i, _ts[i]);
        setts(nnins, n, i, _ts[i]);
    }
    for (i = 0; i < nnins->output_n; i++) {
        printf("tv: %d %d %f\n", n, i, _tv[i]);
        settv(nnins, n, i, _tv[i]);
    }
}


void nn_init(nn_sys_t *nnins, int _input_n, int _hidden_n, int _output_n, int initial_tnum, float _alpha, float _offset)
{
    int i, j;

    nnins->teacher_n = 0;
    nnins->teacher_n_max = initial_tnum;
    nnins->input_n = _input_n;
    nnins->output_n = _output_n;
    nnins->hidden_n = _hidden_n;
    nnins->alpha = _alpha;
    nnins->offset = _offset;

    nnins->ih_w = malloc(sizeof(float*)*nnins->hidden_n);
    for (i = 0; i < nnins->hidden_n; i++) 
        nnins->ih_w[i] = malloc(sizeof(float)*nnins->input_n);
    
    nnins->ho_w = malloc(sizeof(float*)*nnins->output_n);
    for (i = 0; i < nnins->output_n; i++) 
        nnins->ho_w[i] = malloc(sizeof(float)*nnins->hidden_n);

    nnins->ts = malloc(sizeof(float)*nnins->input_n*nnins->teacher_n_max);
    nnins->tv = malloc(sizeof(float)*nnins->output_n*nnins->teacher_n_max);

    nnins->input = malloc(sizeof(float)*nnins->input_n);
    nnins->hidden = malloc(sizeof(float)*nnins->hidden_n);
    nnins->output = malloc(sizeof(float)*nnins->output_n);

    srand((unsigned)time(NULL));
    for (i = 0; i < nnins->hidden_n; i++)
        for (j = 0; j < nnins->input_n; j++)
            nnins->ih_w[i][j] = (float)(rand() - RAND_MAX / 2) * 2 / RAND_MAX;
    for (i = 0; i < nnins->output_n; i++)
        for (j = 0; j < nnins->hidden_n; j++)
             nnins->ho_w[i][j] = (float)(rand() - RAND_MAX / 2) * 2 / RAND_MAX;
}

void nn_free(nn_sys_t *nnins)
{
    int i;
    for (i = 0; i < nnins->hidden_n; i++) 
        free(nnins->ih_w[i]);
    for (i = 0; i < nnins->output_n; i++) 
        free(nnins->ho_w[i]);
    free(nnins->ts);
    free(nnins->tv);
    free(nnins->input);
    free(nnins->hidden);
    free(nnins->output);
}

float sigmoid(float input)
{
    return 1.0 / (1.0 + exp(-input));
}

// 与えられた入力ベクトルを用いて、inputとhiddenとoutputを更新する
void nn_compute_with_ts(nn_sys_t *nnins, int index)
{
    int i, j;
    
    // 入力層が隠れ層に出力した値を覚えておく。
    for (i = 0; i < nnins->input_n; i++)
        nnins->input[i] = getts(nnins, index, i);

    // 入力層からの出力を元に隠れ層の出力を計算する。
    for (i = 0; i < nnins->hidden_n; i++) {
        float sum = 0.0;
        for (j = 0; j < nnins->input_n; j++)
            sum += nnins->ih_w[i][j] * nnins->input[j];
        sum = sigmoid(sum);
        // 隠れ層が出力層に出力した値を覚えておく。
        nnins->hidden[i] = sum;
    }

    // 隠れ層からの出力を元に出力層の出力を計算する。
    for (i = 0; i < nnins->output_n; i++) {
        float sum = 0.0;
        for (j = 0; j < nnins->hidden_n; j++)
            sum += nnins->ho_w[i][j] * nnins->hidden[j];
        sum = sigmoid(sum);

        // 出力層に現れた値を覚えておく。
        nnins->output[i] = sum;
    }
}

// 与えられた入力ベクトルを用いて、inputとhiddenとoutputを更新する
void nn_compute(nn_sys_t *nnins, float *inputarr)
{
    int i, j;
    
    // 入力層が隠れ層に出力した値を覚えておく。
    for (i = 0; i < nnins->input_n; i++)
        nnins->input[i] = inputarr[i];

    // 入力層からの出力を元に隠れ層の出力を計算する。
    for (i = 0; i < nnins->hidden_n; i++) {
        float sum = 0.0;
        for (j = 0; j < nnins->input_n; j++)
            sum += nnins->ih_w[i][j] * nnins->input[j];
        sum = sigmoid(sum);
        // 隠れ層が出力層に出力した値を覚えておく。
        nnins->hidden[i] = sum;
    }

    // 隠れ層からの出力を元に出力層の出力を計算する。
    for (i = 0; i < nnins->output_n; i++) {
        float sum = 0.0;
        for (j = 0; j < nnins->hidden_n; j++)
            sum += nnins->ho_w[i][j] * nnins->hidden[j];
        sum = sigmoid(sum);

        // 出力層に現れた値を覚えておく。
        nnins->output[i] = sum;
    }
}


void nn_bp(nn_sys_t *nnins, int index)
{
    int i, j, k;

    // 与えられた入力ベクトルを用いて、inputとhiddenとoutputを更新する
    nn_compute_with_ts(nnins, index);

    //!< 出力層から隠れ層への伝播
    for (i = 0; i < nnins->output_n; i++) {
        float ep = (nnins->output[i] - gettv(nnins, index, i)) * nnins->output[i] * (1 - nnins->output[i]);
        for (j = 0; j < nnins->hidden_n; j++)
            nnins->ho_w[i][j] -= (nnins->alpha * ep * nnins->hidden[j] + nnins->offset * SGN(ep * nnins->hidden[j]));
    }

    //!< 隠れ層から入力層への伝播
    for (i = 0; i < nnins->hidden_n; i++) {
        float sum = 0.0;
        float ep; 

        for (k = 0; k < nnins->output_n; k++) 
            sum += (nnins->output[k] - gettv(nnins, index, k)) * nnins->output[k] * (1 - nnins->output[k]) * nnins->ho_w[k][i];
        ep = sum * nnins->hidden[i] * (1 - nnins->hidden[i]);

        for (j = 0; j < nnins->input_n; j++) 
            nnins->ih_w[i][j] -= (nnins->alpha * ep * nnins->input[j] + nnins->offset * SGN(ep * nnins->input[j]));
    }
}

float nn_calcerr(nn_sys_t *nnins, int index)
{
    float e = 0.0;
    int i;

    for (i = 0; i < nnins->output_n; i++) 
        e += (gettv(nnins, index, i) - nnins->output[i]) * (gettv(nnins, index, i) - nnins->output[i]);

    return e;
}

