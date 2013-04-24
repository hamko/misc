#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "newkfsys.h"

int kfsys_init(kfsys_t* kfsys, int sdim, int mdim, int cdim, double time, double dt, matrix_t* H, matrix_t* initX, matrix_t* initP, matrix_t* Q)
{
    kfsys->sdim = sdim;
    kfsys->mdim = mdim;
    kfsys->cdim = cdim;
    kfsys->time = time;
    kfsys->dt = dt;

    kfsys->X = initX;
    matrix_retain(initX);
    kfsys->P = initP;
    matrix_retain(initP);
    kfsys->Q = Q;
    matrix_retain(Q);
    kfsys->H = H;
    matrix_retain(H);
    
    matrix_begin();
    kfsys->preX = matrix_copy(initX);
    matrix_retain(kfsys->preX);
    kfsys->preP = matrix_copy(initP);
    matrix_retain(kfsys->preP);
    kfsys->K = matrix_matrix(kfsys->sdim, kfsys->mdim);
    matrix_fill_zero(kfsys->K);
    matrix_retain(kfsys->K);
    matrix_end();

    return 0;
}

int kfsys_free(kfsys_t* kfsys)
{
    matrix_release(kfsys->X);
    matrix_release(kfsys->P);
    matrix_release(kfsys->Q);
    matrix_release(kfsys->H);
    matrix_release(kfsys->K);
    matrix_release(kfsys->preX);
    matrix_release(kfsys->preP);

    return 0;
}

int kfsys_revise(kfsys_t* kfsys, matrix_t* A, matrix_t* B, matrix_t* U, matrix_t* Z, matrix_t* R)
{
    matrix_begin(); // begin

    matrix_autorelease(kfsys->preX);
    matrix_autorelease(kfsys->preP);
    matrix_autorelease(kfsys->K);
    matrix_autorelease(kfsys->X);
    matrix_autorelease(kfsys->P);
#ifdef __DEBUG_PRINT_KFSYS__
    fprintf(stderr, "preX\n");
    matrix_print(kfsys->preX);
    fprintf(stderr, "preP\n");
    matrix_print(kfsys->preP);
    fprintf(stderr, "K\n");
    matrix_print(kfsys->K);
    fprintf(stderr, "X\n");
    matrix_print(kfsys->X);
    fprintf(stderr, "P\n");
    matrix_print(kfsys->P);
    fprintf(stderr, "A\n");
    matrix_print(A);
    fprintf(stderr, "B\n");
    matrix_print(B);
    fprintf(stderr, "U\n");
    matrix_print(U);
    fprintf(stderr, "Z\n");
    matrix_print(Z);
    fprintf(stderr, "R\n");
    matrix_print(R);
    fprintf(stderr, "H\n");
    matrix_print(kfsys->H);
#endif

    // Project the state ahead
    kfsys->preX = matrix_operate_plus(matrix_product(A, kfsys->X), matrix_product(B, U));
    
    // Project the error covariance ahead
    kfsys->preP = matrix_operate_plus(matrix_product(matrix_product(A, kfsys->P), matrix_transpose(A)), kfsys->Q);

    // Compute the Karman gain
    kfsys->K = matrix_product(kfsys->preP, matrix_product(matrix_transpose(kfsys->H), matrix_inverse(
                    matrix_operate_plus(matrix_product(kfsys->H, (matrix_product(kfsys->preP, matrix_transpose(kfsys->H)))), R)
                    )));


    // Update estimate with measurement Z
    kfsys->X = matrix_operate_plus(kfsys->preX, matrix_product(kfsys->K, matrix_operate_minus(Z, matrix_product(kfsys->H, kfsys->preX))));


    // Update the error covariance
    kfsys->P = matrix_product(matrix_operate_minus(matrix_unit(kfsys->sdim), matrix_product(kfsys->K, kfsys->H)), kfsys->preP);

    matrix_retain(kfsys->preX);
    matrix_retain(kfsys->preP);
    matrix_retain(kfsys->K);
    matrix_retain(kfsys->X);
    matrix_retain(kfsys->P);

#ifdef __DEBUG_PRINT_KFSYS__
    fprintf(stderr, "postX\n");
    matrix_print(kfsys->X);
    fprintf(stderr, "postpreX\n");
    matrix_print(kfsys->preX);
#endif
    matrix_end();
    return 0;
}

matrix_t* kfsys_getX(kfsys_t* kfsys)
{
    return kfsys->X;
}

matrix_t* kfsys_getP(kfsys_t* kfsys)
{
    return kfsys->P;
}

double kfsys_gettime(kfsys_t* kfsys)
{
    return kfsys->time;
}
