#ifndef __KFSYS_H_INCLUDED__
#define __KFSYS_H_INCLUDED__

#include "matrix.h"

#define __DEBUG_PRINT_KFSYS__

typedef struct _kfsys_t {
    int sdim;   // state vector dim
    int mdim;   // measurement vector dim
    int cdim;   // control vector dim
    double time;
    double dt;
    matrix_t* X;   // s*1 state vector
    matrix_t* H;   // m*s state vector
    matrix_t* P;   // s*s cov error matrix
    matrix_t* Q;   // s*s process error cov.
//    matrix_t* R;   // m*m measurement error cov.
    // tmp matrix
    matrix_t* preX;    // s*1 priori state vector.
    matrix_t* preP;    // s*s priori cov error matrix.
    matrix_t* K;       // s*m karman gain matrix.
} kfsys_t;

//! @params
//! initX   s*1 initial state.
//! initP   s*s initial cov.
//! H   m*s matrix from state to measure.
//! Q   s*s process error cov. (using BoxMuller)
extern int kfsys_init(kfsys_t* kfsys, int sdim, int mdim, int cdim, double time, double dt, matrix_t* H, matrix_t* initX, matrix_t* initP, matrix_t* Q);

// robolib_matrixのfreeをしないといけない
extern int kfsys_free(kfsys_t* kfsys);

//! @params
//! A   s*s state change matrix.
//! B   s*c control coefficient matrix.
//! U   c*1 control vector.
//! Z   m*1 measurement vector
//! R   m*m measurement error cov. (using BoxMuller)
extern int kfsys_revise(kfsys_t* kfsys, matrix_t* A, matrix_t* B, matrix_t* U, matrix_t* Z, matrix_t* R);

extern matrix_t* kfsys_getX(kfsys_t* kfsys);
extern matrix_t* kfsys_getP(kfsys_t* kfsys);
extern double kfsys_gettime(kfsys_t* kfsys);

#endif
