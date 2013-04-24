#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "matrix.h"
#include "matrix_test.h"

extern releasepool_t* pool;
/*******************/

static void is_equal_int(const char* str, int result, int expected)
{
    if (result == expected)
        printf("%-8sPASSED.\n", str);
    else 
        printf("%-8sFAILED.\nresult: %d, expected: %d\n", str, result, expected);
}

void verify_matrix_alloc(void)
{
    printf("verify_matrix_alloc\n");
    matrix_t* a = matrix_alloc(2, 3);
    is_equal_int("row", a->rows, 2);
    is_equal_int("col", a->cols, 3);
    matrix_free(a);
}
void verify_compare_with_precision(void)
{
    printf("verify_compare_with_precision\n");
    matrix_begin();
    matrix_t* a = matrix_matrix(2, 3);
    matrix_t* b = matrix_matrix(2, 3);
    matrix_t* c = matrix_matrix(2, 3);
    ELEMENT(a, 0, 0) = 1.01; ELEMENT(a, 0, 1) = 1; ELEMENT(a, 0, 2) = 1;
    ELEMENT(a, 1, 0) = 2; ELEMENT(a, 1, 1) = 2; ELEMENT(a, 1, 2) = 2;
    ELEMENT(b, 0, 0) = 1; ELEMENT(b, 0, 1) = 1; ELEMENT(b, 0, 2) = 1;
    ELEMENT(b, 1, 0) = 2; ELEMENT(b, 1, 1) = 2; ELEMENT(b, 1, 2) = 2;
    ELEMENT(c, 0, 0) = 1.000000000001; ELEMENT(c, 0, 1) = 1; ELEMENT(c, 0, 2) = 1;
    ELEMENT(c, 1, 0) = 2; ELEMENT(c, 1, 1) = 2; ELEMENT(c, 1, 2) = 2;
    is_equal_int("ab 0", matrix_compare_with_precision(a, b, 0), 0);
    is_equal_int("ab 1e-3", matrix_compare_with_precision(a, b, 1.0e-3), 0);
    is_equal_int("ab 1e-1", matrix_compare_with_precision(a, b, 1.0e-1), 1);
    is_equal_int("ab", matrix_compare(a, b), 0);
    is_equal_int("bc", matrix_compare(b, c), 1);
    matrix_end();
}

void verify_transpose(void)
{
    printf("verify_transpose\n");

    matrix_begin();
    matrix_t* a = matrix_matrix(2, 3);
    matrix_t* c = matrix_matrix(3, 2);
    ELEMENT(a, 0, 0) = 1; ELEMENT(a, 0, 1) = 2; ELEMENT(a, 0, 2) = 3;
    ELEMENT(a, 1, 0) = 4; ELEMENT(a, 1, 1) = 5; ELEMENT(a, 1, 2) = 6;
    ELEMENT(c, 0, 0) = 1; ELEMENT(c, 0, 1) = 4;
    ELEMENT(c, 1, 0) = 2; ELEMENT(c, 1, 1) = 5;
    ELEMENT(c, 2, 0) = 3; ELEMENT(c, 2, 1) = 6;

    matrix_t* b = matrix_transpose(a);

    is_equal_int("b^T", matrix_compare(b, c), 1);

    matrix_end();
}

void verify_connect(void)
{
    printf("verify_connect\n");
    matrix_begin();

    matrix_t* a = matrix_matrix(3, 3);
    matrix_t* b = matrix_matrix(2, 3);
    matrix_t* c = matrix_matrix(2, 2);
    matrix_t* d = matrix_matrix(5, 3);
    matrix_t* e = matrix_matrix(2, 5);
    ELEMENT(a, 0, 0) = 1; ELEMENT(a, 0, 1) = 2; ELEMENT(a, 0, 2) = 3;
    ELEMENT(a, 1, 0) = 4; ELEMENT(a, 1, 1) = 5; ELEMENT(a, 1, 2) = 6;
    ELEMENT(a, 2, 0) = 0; ELEMENT(a, 2, 1) = 2; ELEMENT(a, 2, 2) = 7;
    ELEMENT(b, 0, 0) = 8; ELEMENT(b, 0, 1) = 5; ELEMENT(b, 0, 2) = 2;
    ELEMENT(b, 1, 0) = 1; ELEMENT(b, 1, 1) = 6; ELEMENT(b, 1, 2) = 0;
    ELEMENT(c, 0, 0) = 1; ELEMENT(c, 0, 1) = 2;
    ELEMENT(c, 1, 0) = 4; ELEMENT(c, 1, 1) = 5;
    ELEMENT(d, 0, 0) = 1; ELEMENT(d, 0, 1) = 2; ELEMENT(d, 0, 2) = 3;
    ELEMENT(d, 1, 0) = 4; ELEMENT(d, 1, 1) = 5; ELEMENT(d, 1, 2) = 6;
    ELEMENT(d, 2, 0) = 0; ELEMENT(d, 2, 1) = 2; ELEMENT(d, 2, 2) = 7;
    ELEMENT(d, 3, 0) = 8; ELEMENT(d, 3, 1) = 5; ELEMENT(d, 3, 2) = 2;
    ELEMENT(d, 4, 0) = 1; ELEMENT(d, 4, 1) = 6; ELEMENT(d, 4, 2) = 0;
    ELEMENT(e, 0, 0) = 8; ELEMENT(e, 0, 1) = 5; ELEMENT(e, 0, 2) = 2; ELEMENT(e, 0, 3) = 1; ELEMENT(e, 0, 4) = 2;
    ELEMENT(e, 1, 0) = 1; ELEMENT(e, 1, 1) = 6; ELEMENT(e, 1, 2) = 0; ELEMENT(e, 1, 3) = 4; ELEMENT(e, 1, 4) = 5;

    matrix_t* ab = matrix_rows_connect(a, b);
    matrix_t* bc = matrix_cols_connect(b, c);
    is_equal_int("(ab)=d", matrix_compare(ab, d), 1);
    is_equal_int("(bc)=e", matrix_compare(bc, e), 1);

    matrix_end();
}

void verify_mult_connect(void)
{
    printf("verify_mult_connect\n");

    matrix_t* a = matrix_alloc(3, 3);
    matrix_t* b = matrix_alloc(2, 3);
    matrix_t* f = matrix_alloc(2, 3);
    matrix_t* c = matrix_alloc(2, 2);

    matrix_t* d = matrix_alloc(7, 3);    // a, b, f
    matrix_t* e = matrix_alloc(2, 8);    // b, f, c

    ELEMENT(a, 0, 0) = 1; ELEMENT(a, 0, 1) = 2; ELEMENT(a, 0, 2) = 3;
    ELEMENT(a, 1, 0) = 4; ELEMENT(a, 1, 1) = 5; ELEMENT(a, 1, 2) = 6;
    ELEMENT(a, 2, 0) = 0; ELEMENT(a, 2, 1) = 2; ELEMENT(a, 2, 2) = 7;
    ELEMENT(b, 0, 0) = 8; ELEMENT(b, 0, 1) = 5; ELEMENT(b, 0, 2) = 2;
    ELEMENT(b, 1, 0) = 1; ELEMENT(b, 1, 1) = 6; ELEMENT(b, 1, 2) = 0;
    ELEMENT(f, 0, 0) = 6; ELEMENT(f, 0, 1) = 9; ELEMENT(f, 0, 2) = 1;
    ELEMENT(f, 1, 0) = 7; ELEMENT(f, 1, 1) = 3; ELEMENT(f, 1, 2) = 3;
    ELEMENT(c, 0, 0) = 1; ELEMENT(c, 0, 1) = 2;
    ELEMENT(c, 1, 0) = 4; ELEMENT(c, 1, 1) = 5;

    ELEMENT(d, 0, 0) = 1; ELEMENT(d, 0, 1) = 2; ELEMENT(d, 0, 2) = 3;
    ELEMENT(d, 1, 0) = 4; ELEMENT(d, 1, 1) = 5; ELEMENT(d, 1, 2) = 6;
    ELEMENT(d, 2, 0) = 0; ELEMENT(d, 2, 1) = 2; ELEMENT(d, 2, 2) = 7;
    ELEMENT(d, 3, 0) = 8; ELEMENT(d, 3, 1) = 5; ELEMENT(d, 3, 2) = 2;
    ELEMENT(d, 4, 0) = 1; ELEMENT(d, 4, 1) = 6; ELEMENT(d, 4, 2) = 0;
    ELEMENT(d, 5, 0) = 6; ELEMENT(d, 5, 1) = 9; ELEMENT(d, 5, 2) = 1;
    ELEMENT(d, 6, 0) = 7; ELEMENT(d, 6, 1) = 3; ELEMENT(d, 6, 2) = 3;

    ELEMENT(e, 0, 0) = 8; ELEMENT(e, 0, 1) = 5; ELEMENT(e, 0, 2) = 2; ELEMENT(e, 0, 3) = 1; ELEMENT(e, 0, 4) = 2; ELEMENT(e, 0, 5) = 6; ELEMENT(e, 0, 6) = 9; ELEMENT(e, 0, 7) = 1;
    ELEMENT(e, 1, 0) = 1; ELEMENT(e, 1, 1) = 6; ELEMENT(e, 1, 2) = 0; ELEMENT(e, 1, 3) = 4; ELEMENT(e, 1, 4) = 5; ELEMENT(e, 1, 5) = 7; ELEMENT(e, 1, 6) = 3; ELEMENT(e, 1, 7) = 3;

    matrix_t* abf_arr[3] = {a, b, f};
    matrix_t* bcf_arr[3] = {b, c, f};

    matrix_begin(); // begin

    matrix_t* abf = matrix_multrows_connect(abf_arr, 3);
    matrix_t* bcf = matrix_multcols_connect(bcf_arr, 3);
    
    is_equal_int("(abf)=d", matrix_compare(abf, d), 1);
    is_equal_int("(bcf)=e", matrix_compare(bcf, e), 1);

    matrix_end(); // end

    matrix_free(a);
    matrix_free(b);
    matrix_free(c);
    matrix_free(d);
    matrix_free(e);
    matrix_free(f);
}

#define ABS(a) ((a)>0?(a):-(a))
#define is_simular(a,b) (ABS((a)-(b))<(1.0e-5))
void verify_matrix_quadratic(void)
{
    printf("verify_quadratic\n");

    matrix_t* a = matrix_alloc(3, 1);
    matrix_t* b = matrix_alloc(3, 3);
    matrix_t* c = matrix_alloc(3, 1);

    ELEMENT(a, 0, 0) = 1; ELEMENT(a, 1, 0) = 3; ELEMENT(a, 2, 0) = 5; 
    ELEMENT(b, 0, 0) = 1; ELEMENT(b, 0, 1) = 3; ELEMENT(b, 0, 2) = 1; 
    ELEMENT(b, 1, 0) = 3; ELEMENT(b, 1, 1) = 2; ELEMENT(b, 1, 2) = 5; 
    ELEMENT(b, 2, 0) = 2; ELEMENT(b, 2, 1) = 5; ELEMENT(b, 2, 2) = 2; 
    ELEMENT(c, 0, 0) = 1; ELEMENT(c, 1, 0) = 3; ELEMENT(c, 2, 0) = 5; 

    is_equal_int("quad", is_simular(matrix_quadratic(a, b, c), 252), 1);

    matrix_free(a);
    matrix_free(b);
    matrix_free(c);
}

void verify_matrix_pseudo_inverse()
{
    printf("verify_quadratic\n");

    matrix_begin();
    matrix_t* a = matrix_matrix(3, 2);
    matrix_t* c = matrix_matrix(2, 3);
    matrix_t* i2 = matrix_matrix(2, 2);

    ELEMENT(a, 0, 0) = 1; ELEMENT(a, 0, 1) = 4;
    ELEMENT(a, 1, 0) = 2; ELEMENT(a, 1, 1) = 5;
    ELEMENT(a, 2, 0) = 3; ELEMENT(a, 2, 1) = 6;

    ELEMENT(c, 0, 0) = 0; ELEMENT(c, 0, 1) = 4; ELEMENT(c, 0, 2) = 2;
    ELEMENT(c, 1, 0) = 1; ELEMENT(c, 1, 1) = 5; ELEMENT(c, 1, 2) = 3;

    ELEMENT(i2, 0, 0) = 1; ELEMENT(i2, 0, 1) = 0;
    ELEMENT(i2, 1, 0) = 0; ELEMENT(i2, 1, 1) = 1;

    matrix_t* b = matrix_pseudo_inverse(a);
    matrix_t* ab = matrix_product(b, a);
    matrix_t* d = matrix_pseudo_inverse(c);
    matrix_t* cd = matrix_product(c, d);
    is_equal_int("ba=i2", matrix_compare_with_precision(ab, i2, 1.0e-5), 1);
    is_equal_int("cd=i2", matrix_compare_with_precision(cd, i2, 1.0e-5), 1);

    matrix_end(); // end
}

void verify_matrix(void)
{
    verify_matrix_alloc();
    verify_compare_with_precision();
    verify_transpose();
    verify_connect();
    verify_mult_connect();
    verify_matrix_quadratic();
    verify_matrix_pseudo_inverse();
}
