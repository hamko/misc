#ifndef __NNSYS_H_INCLUDED__
#define __NNSYS_H_INCLUDED__

typedef struct _nn_sys_t {
    int teacher_n; //!< 教師信号・ベクトルの組の数
    int teacher_n_max; //!< teacher_nのallocのmax

    int input_n ;  //!< 入力層のユニットの数
    int hidden_n;  //!< 隠れ層のユニットの数
    int output_n;  //!< 出力層のユニットの数
    float alpha;
    float offset;

    //!< 入力層から隠れ層への重み係数配列
    float **ih_w;
    //!< 隠れ層から出力層への重み係数配列
    float **ho_w;
    //!< 各ユニットが出力した値を一時的に保持するための配列
    float *output;
    float *hidden;
    float *input;

    float *ts;   // ts[i][j]はi番目の教師の入力ベクトル
    float *tv;  // tv[i][j]はi番目の教師の出力ベクトル
} nn_sys_t;

/* 数学関数、マクロ */ 
#define SGN(a) (a > 0 ? 1 : -1)
float sigmoid(float input);

void nn_init(nn_sys_t *nnins, int _input_n, int _hidden_n, int _output_n, int initial_tnum, float _alpha, float _offset);
void nn_free(nn_sys_t *nnins);

void settv(nn_sys_t *nnins, int teacher, int i, float val);
float gettv(nn_sys_t *nnins, int teacher, int i);
void setts(nn_sys_t *nnins, int teacher, int i, float val);
float getts(nn_sys_t *nnins, int teacher, int i);

void nn_add_teacher_vect(nn_sys_t *nnins, float *_ts, float *_tv);

void nn_compute_with_ts(nn_sys_t *nnins, int index);
void nn_compute(nn_sys_t *nnins, float* inputarr);
void nn_bp(nn_sys_t *nnins, int index);
float nn_calcerr(nn_sys_t *nnins, int index);

#endif
