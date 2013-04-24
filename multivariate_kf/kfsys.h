#ifndef __KFSYS_H_INCLUDED__
#define __KFSYS_H_INCLUDED__

#include "robolib_matrix.h"

typedef struct _kfsys_t {
    int sdim;   // state vector dim
    int mdim;   // measurement vector dim
    int cdim;   // control vector dim
    double time;
    double dt;
    mat_t* X;   // s*1 state vector
    mat_t* H;   // m*s state vector
    mat_t* P;   // s*s cov error matrix
    mat_t* Q;   // s*s process error cov.
//    mat_t* R;   // m*m measurement error cov.
    // tmp matrix
    mat_t* preX;    // s*1 priori state vector.
    mat_t* preP;    // s*s priori cov error matrix.
    mat_t* K;       // s*m karman gain matrix.
} kfsys_t;

//! @params
//! initX   s*1 initial state.
//! initP   s*s initial cov.
//! H   m*s matrix from state to measure.
//! Q   s*s process error cov. (using BoxMuller)
extern int kfsys_init(kfsys_t* kfsys, int sdim, int mdim, int cdim, double time, double dt, const mat_t* H, const mat_t* initX, const mat_t* initP, const mat_t* Q);

// robolib_matrixのfreeをしないといけない
extern int kfsys_free(kfsys_t* kfsys);

//! @params
//! A   s*s state change matrix.
//! B   s*c control coefficient matrix.
//! U   c*1 control vector.
//! Z   m*1 measurement vector
//! R   m*m measurement error cov. (using BoxMuller)
extern int kfsys_revise(kfsys_t* kfsys, const mat_t* A, const mat_t* B, const mat_t* U, const mat_t* Z, const mat_t* R);

extern mat_t* kfsys_getX(const kfsys_t* kfsys);
extern mat_t* kfsys_getP(const kfsys_t* kfsys);
extern double kfsys_gettime(const kfsys_t* kfsys);

#endif
