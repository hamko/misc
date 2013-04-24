#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "question2_another.h"

/**************/
// このファイルは正しいので書き換えないこと！
// 書き換えるのはquestion2_another.cのsq_matrix_reviseとsq_init_matrixのみ。
/**************/

void init_mat(void)
{
    matrix_begin();
    matrix_t* A = matrix_matrix(2, 2);
    ELEMENT(A, 0, 0) = 1.0; ELEMENT(A, 0, 0) = 3.0; 
    ELEMENT(A, 0, 0) = 2.0; ELEMENT(A, 0, 0) = 4.0; 
    sq_init_matrix(A);
    matrix_end();
}

int main(void)
{
    init_mat();
    sq_matrix_revise();
    sq_matrix_print();
    sq_release();

    return 0;
}
