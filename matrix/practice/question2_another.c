#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

static matrix_t* A;

void sq_init_matrix(matrix_t* _A)
{
    A = _A; // この式だけではだめなので、書き換える
}

// Aの二乗をAに入れる関数
void sq_matrix_revise(void)
{
    A = matrix_product(A, A); // この式だけではダメなので書き換える
}

// この関数は正しいので書き換えないこと
void sq_matrix_print(void)
{
    matrix_print(A);
}

// この関数は正しいので書き換えないこと
void sq_release(void)
{
    matrix_release(A);
}
