#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__

typedef double value_t;

typedef struct _matrix {
	int retainCount;
	int rows;
	int cols;
	value_t* data;
} matrix_t;

typedef struct releasepool {
	matrix_t* object;
	struct releasepool* next;
} releasepool_t;

extern matrix_t* matrix_autorelease(matrix_t* p);
extern void matrix_end();
extern void matrix_begin();


#if MATRIX_DEGUG
#define MATRIX_ASSERT(m, r, c) matrix_assert(m, r, c)
#else
#define MATRIX_ASSERT(m, r, c) 
#endif

#define ELEMENT(m, r, c)	((m)->data[(r) * (m)->cols + (c)])

extern void matrix_assert(matrix_t* a, int r, int c);
extern matrix_t* matrix_alloc(int rows, int cols);
extern matrix_t* matrix_matrix(int rows, int cols);

extern int matrix_compare_with_precision(matrix_t* a, matrix_t* b, value_t precision);
extern int matrix_compare(matrix_t* a, matrix_t* b);

extern matrix_t* matrix_unit(int dim);
extern matrix_t* matrix_minus(matrix_t* a);
extern matrix_t* matrix_exterior(matrix_t* a);
extern matrix_t* matrix_rows_connect(matrix_t* a, matrix_t* b);
extern matrix_t* matrix_cols_connect(matrix_t* a, matrix_t* b);
extern matrix_t* matrix_multcols_connect(matrix_t** a, int matrix_num);
extern matrix_t* matrix_multrows_connect(matrix_t** a, int matrix_num);
extern matrix_t* matrix_linear_comb(value_t aval, matrix_t* a, value_t bval, matrix_t* b);
extern matrix_t* matrix_operate_plus(matrix_t* a, matrix_t* b);
extern matrix_t* matrix_operate_minus(matrix_t* a, matrix_t* b);
extern matrix_t* matrix_copy(matrix_t* a);
extern matrix_t* matrix_scolar_product(value_t val, matrix_t* a);
extern matrix_t* matrix_product(matrix_t* a, matrix_t* b);
extern matrix_t* matrix_transpose(matrix_t* a);

// 0行列に初期化
extern void matrix_fill_zero(matrix_t* mat);
// 二次形式
extern value_t matrix_quadratic(matrix_t* a, matrix_t* b, matrix_t* c);
// 行列式
extern value_t matrix_det(matrix_t* m);
extern matrix_t* matrix_inverse(matrix_t* a);
extern matrix_t* matrix_pseudo_inverse(matrix_t* m);

void matrix_retain(matrix_t* a);
extern void matrix_free(matrix_t* a);
extern void matrix_release(matrix_t* a);

extern releasepool_t* new_node();
extern void free_node(releasepool_t* p);
extern void matrix_print(matrix_t* p);
#endif
