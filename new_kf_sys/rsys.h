#ifndef __RSYS_H_INCLUDED__
#define __RSYS_H_INCLUDED__

typedef double (*rsysfunc_t)(double*);
typedef struct _rungesys_t {
    int dim;    // 状態変数の次元
    rsysfunc_t* funcarr; // 各状態に関する更新式
    double* state; // 状態変数の次元+1(時間)の配列
    double dt;
} rungesys_t;

int rsys_init(rungesys_t* rsys, int dim, rsysfunc_t* funcarr, double* initarr, double dt);
int rsys_revise(rungesys_t* rsys);
double* rsys_get_state(rungesys_t* rsys);

#endif 
