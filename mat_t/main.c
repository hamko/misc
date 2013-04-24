#include <stdio.h>
#include "robolib_matrix.h"
#include "dfree.h"

int main(void)
{
    mat_t* A = robolib_matrix_alloc(3, 1);
    mat_t* B = robolib_matrix_alloc(1, 3);
    mat_t* tmp = robolib_matrix_alloc(0, 0);
    robolib_matrix_set(A, 0, 0, 1.0);
    robolib_matrix_set(A, 1, 0, 2.0);
    robolib_matrix_set(A, 2, 0, 3.0);
    robolib_matrix_set(B, 0, 0, 1.0);
    robolib_matrix_set(B, 0, 1, 2.0);
    robolib_matrix_set(B, 0, 2, 3.0);

    tmp = robolib_matrix_product(A, B, tmp);
    robolib_matrix_print(tmp);
    mat_t* I_3 = robolib_matrix_new_unit(3);
    tmp = robolib_matrix_linear_comb(1.0, tmp, 1.0, I_3, tmp);
    robolib_matrix_print(tmp);
    tmp = robolib_matrix_inverse(tmp, tmp);
    robolib_matrix_print(tmp);
    
    robolib_matrix_free(A);
    robolib_matrix_free(B);
    robolib_matrix_free(tmp);
}
