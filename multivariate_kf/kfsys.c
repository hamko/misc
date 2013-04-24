#include <stdio.h>
#include <stdlib.h>
#include "robolib_matrix.h"
#include "kfsys.h"

int kfsys_init(kfsys_t* kfsys, int sdim, int mdim, int cdim, double time, double dt, const mat_t* H, const mat_t* initX, const mat_t* initP, const mat_t* Q)
{
    kfsys->sdim = sdim;
    kfsys->mdim = mdim;
    kfsys->cdim = cdim;
    kfsys->time = time;
    kfsys->dt = dt;

    kfsys->X = robolib_column_vector_alloc(sdim);
    kfsys->X = robolib_matrix_copy(initX, kfsys->X);
    kfsys->P = robolib_matrix_alloc(sdim, sdim);
    kfsys->P = robolib_matrix_copy(initP, kfsys->P);
    kfsys->Q = robolib_matrix_alloc(sdim, sdim);
    kfsys->Q = robolib_matrix_copy(Q, kfsys->Q);
    kfsys->H = robolib_matrix_alloc(mdim, mdim);
    kfsys->H = robolib_matrix_copy(H, kfsys->H);
    
    kfsys->preX = robolib_column_vector_alloc(kfsys->sdim);
    kfsys->preP = robolib_matrix_alloc(kfsys->sdim, kfsys->sdim);
    kfsys->K = robolib_matrix_alloc(kfsys->sdim, kfsys->mdim);

    return 0;
}

int kfsys_free(kfsys_t* kfsys)
{
    robolib_matrix_free(kfsys->X);
    robolib_matrix_free(kfsys->P);
    robolib_matrix_free(kfsys->Q);
    robolib_matrix_free(kfsys->H);
    robolib_matrix_free(kfsys->K);
    robolib_matrix_free(kfsys->preX);
    robolib_matrix_free(kfsys->preP);

    return 0;
}

int kfsys_revise(kfsys_t* kfsys, const mat_t* A, const mat_t* B, const mat_t* U, const mat_t* Z, const mat_t* R)
{
    int i, j;
    mat_t* tmp = robolib_matrix_alloc(0, 0);
    mat_t* tmp2 = robolib_matrix_alloc(0, 0);

    // Project the state ahead
    tmp = robolib_matrix_product(A, kfsys->X, tmp);
//    fprintf(stderr, "1 tmp: %p\n", tmp);
    tmp2 = robolib_matrix_product(B, U, tmp2);
//    fprintf(stderr, "2 tmp2: %p\n", tmp2);
    kfsys->preX = robolib_matrix_linear_comb(1.0, tmp, 1.0, tmp2, kfsys->preX);
//    fprintf(stderr, "2 preX: %p\n", kfsys->preX);
    
    // Project the error covariance ahead
    tmp = robolib_matrix_trans(A, tmp);
//    fprintf(stderr, "1 tmp: %p\n", tmp);
    tmp2 = robolib_matrix_product(kfsys->P, tmp, tmp2);
//    fprintf(stderr, "2 tmp2: %p\n", tmp2);
    tmp = robolib_matrix_product(A, tmp2, tmp);
//    fprintf(stderr, "1 tmp: %p\n", tmp);
    kfsys->preP = robolib_matrix_linear_comb(1.0, tmp, 1.0, kfsys->Q, kfsys->preP);
//    fprintf(stderr, "2 preX: %p\n", kfsys->preX);

    // Compute the Karman gain
    mat_t* Ht = robolib_matrix_alloc(kfsys->sdim, kfsys->mdim);
    Ht = robolib_matrix_trans(kfsys->H, Ht);
    tmp2 = robolib_matrix_product(kfsys->preP, Ht, tmp2); // prePHt
//    fprintf(stderr, "2 tmp2: %p\n", tmp2);
    tmp = robolib_matrix_product(kfsys->H, tmp2, tmp); // HprePHt
//    fprintf(stderr, "1 tmp: %p\n", tmp);
    tmp2 = robolib_matrix_linear_comb(1.0, tmp, 1.0, R, tmp2); // HprePHt+R
//    fprintf(stderr, "2 tmp2: %p\n", tmp2);
    tmp = robolib_matrix_inverse(tmp2, tmp); // inv(HprePHt+R)
//    fprintf(stderr, "1 tmp: %p\n", tmp); 
    tmp2 = robolib_matrix_product(Ht, tmp, tmp2); // Htinv(HprePHt+R)
    robolib_matrix_print(Ht);
    robolib_matrix_print(tmp);
//    fprintf(stderr, "2 tmp2: %p\n", tmp2);
    kfsys->K = robolib_matrix_product(kfsys->preP, tmp2, kfsys->K); // prePHtinv(HprePHt+R)
//    fprintf(stderr, "2 K: %p\n", kfsys->K);
    robolib_matrix_free(Ht);

    // Update estimate with measurement Z
    tmp = robolib_matrix_product(kfsys->H, kfsys->preX, tmp);
//    fprintf(stderr, "2 tmp: %p\n", tmp);
    tmp2 = robolib_matrix_linear_comb(1.0, Z, -1.0, tmp, tmp2);
//    fprintf(stderr, "2 tmp2: %p\n", tmp2);
    tmp = robolib_matrix_product(kfsys->K, tmp2, tmp);
//    fprintf(stderr, "2 tmp: %p\n", tmp);
    kfsys->X = robolib_matrix_linear_comb(1.0, tmp, 1.0, kfsys->preX, kfsys->X);
//    fprintf(stderr, "3 kfsys->X: %p\n", kfsys->X);

    // Update the error covariance
    mat_t* I = robolib_matrix_new_unit(kfsys->sdim);
    tmp = robolib_matrix_product(kfsys->K, kfsys->H, tmp);
//    fprintf(stderr, "1 tmp: %p\n", tmp);
    tmp2 = robolib_matrix_linear_comb(1.0, I, -1.0, tmp, tmp2);
    kfsys->P = robolib_matrix_product(tmp2, kfsys->preP, kfsys->P);
    robolib_matrix_free(I);

    return 0;
}

mat_t* kfsys_getX(const kfsys_t* kfsys)
{
    return kfsys->X;
}

mat_t* kfsys_getP(const kfsys_t* kfsys)
{
    return kfsys->P;
}

double kfsys_gettime(const kfsys_t* kfsys)
{
    return kfsys->time;
}
