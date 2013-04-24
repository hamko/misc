#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

matrix_t* ABplusC(matrix_t* A, matrix_t* B, matrix_t* C)
{
    // ここになんか書く
    return NULL;
}

// この関数は正しいので書き換えないこと！
int main(void)
{
    matrix_begin();
    matrix_t* A = matrix_matrix(2, 2);
    ELEMENT(A, 0, 0) = 1; ELEMENT(A, 0, 1) = 2;
    ELEMENT(A, 1, 0) = 3; ELEMENT(A, 1, 1) = 4;
    matrix_t* B = matrix_matrix(2, 2);
    ELEMENT(B, 0, 0) = 0; ELEMENT(B, 0, 1) = 2;
    ELEMENT(B, 1, 0) = 2; ELEMENT(B, 1, 1) = 1;
    matrix_t* C = matrix_matrix(2, 2);
    ELEMENT(C, 0, 0) = 1; ELEMENT(C, 0, 1) = 5;
    ELEMENT(C, 1, 0) = 3; ELEMENT(C, 1, 1) = 2;

    matrix_print(ABplusC(A, B, C));
    matrix_end();

    return 0;
}
