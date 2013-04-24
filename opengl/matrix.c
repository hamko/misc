#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "matrix.h"

/*******************/
// auto release functionはbeginとendの間でないと使えない。

#define ABS(a) ((a)>0?(a):-(a))

releasepool_t* pool;
//static releasepool_t* pool;
static matrix_t marker;

void matrix_assert(matrix_t* a, int r, int c)
{
	assert(a != NULL);
	assert(a->data != NULL);
	assert(a->rows > 0);
	assert(a->cols > 0);
	assert(r >= 0);
	assert(c >= 0);
	assert(r < a->rows);
	assert(c < a->rows);
}

// この関数をユーザが使う場合は、責任を持って自分で解放すること。
// もしくはautoreleaseで後回しにすること。
matrix_t* matrix_alloc(int rows, int cols)
{
	matrix_t* r = (matrix_t *)malloc(sizeof(matrix_t));
	r->retainCount = 1;
	r->rows = rows;
	r->cols = cols;
	r->data = (value_t *)malloc(sizeof(value_t) * rows * cols);

    return r;
}

// autorelease function
matrix_t* matrix_matrix(int rows, int cols)
{
	matrix_t* r = (matrix_t *)malloc(sizeof(matrix_t));
	r->retainCount = 1;
	r->rows = rows;
	r->cols = cols;
	r->data = (value_t *)malloc(sizeof(value_t) * rows * cols);

    return matrix_autorelease(r);
}

int matrix_compare_with_precision(matrix_t* a, matrix_t* b, value_t precision)
{
    if (a->rows != b->rows)
        return 0;
    if (a->cols != b->cols)
        return 0;

    int i, j;
    int flag = 1;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            if (ABS(ELEMENT(a, i, j) - ELEMENT(b, i, j)) > precision) {
                flag = 0;
                break;
            }
        }
    }

    return flag;
}

int matrix_compare(matrix_t* a, matrix_t* b)
{
    return matrix_compare_with_precision(a, b, COMPARE_PRECISION);
}

// autorelease function
matrix_t* matrix_minus(matrix_t* a)
{
	matrix_t* r = matrix_alloc(a->rows, a->cols);
    int i;
    for (i = 0; i < a->rows * a->cols; i++)
        r->data[i] = -a->data[i];
    return matrix_autorelease(r);
}

// autorelease function
matrix_t* matrix_unit(int dim)
{
    if (dim < 1) {
        assert(0);
    }

	matrix_t* r = matrix_alloc(dim, dim);
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++)
            ELEMENT(r, i, j) = 0.0;
    for (i = 0; i < dim; i++)
        ELEMENT(r, i, i) = 1;

    return matrix_autorelease(r);
}

// autorelease function
matrix_t* matrix_copy(matrix_t* a)
{
	matrix_t* r = matrix_alloc(a->rows, a->cols);
	memcpy(r->data, a->data, sizeof(value_t) * a->rows * a->cols);
    return matrix_autorelease(r);
}

// autorelease function
matrix_t* matrix_exterior(matrix_t* a)
{
    if (a->rows != 3 || a->cols != 1) {
        assert(0);
    }

	matrix_t* r = matrix_alloc(3, 3);

    ELEMENT(r, 0, 0) = 0; ELEMENT(r, 0, 1) = -ELEMENT(a, 2, 0); ELEMENT(r, 0, 2) = ELEMENT(a, 1, 0);
    ELEMENT(r, 1, 0) = ELEMENT(a, 2, 0); ELEMENT(r, 1, 1) = 0; ELEMENT(r, 1, 2) = -ELEMENT(a, 0, 0);
    ELEMENT(r, 2, 0) = -ELEMENT(a, 1, 0); ELEMENT(r, 2, 1) = ELEMENT(a, 0, 0); ELEMENT(r, 2, 2) = 0;

    return matrix_autorelease(r);
}


// autorelease function
// a:l*nと、b:h*nを(l+h)*n行列に連結。bが下。
matrix_t* matrix_rows_connect(matrix_t* a, matrix_t* b)
{
    if (a->cols != b->cols) {
        assert(0);
    }

	matrix_t* r = matrix_alloc(a->rows + b->rows, a->cols);
	memcpy(r->data, a->data, sizeof(value_t) * a->rows * a->cols);
	memcpy(r->data + a->rows * a->cols, b->data , sizeof(value_t) * b->rows * b->cols);
    return matrix_autorelease(r);
}

// autorelease function
// a[0]からa[matrix_num-1]までを、上から下へと連結した行列を返す
matrix_t* matrix_multrows_connect(matrix_t** a, int matrix_num)
{
    int i;
    int toberow = 0;
    for (i = 0; i < matrix_num - 1; i++) {
        if (a[i]->cols != a[i+1]->cols) {
            assert(0);
        }
        toberow += a[i]->rows;
    }
    toberow += a[matrix_num - 1]->rows;

	matrix_t* r = matrix_alloc(toberow, a[0]->cols);
    double* datap = r->data;
    for (i = 0; i < matrix_num; i++) {
	    memcpy(datap, a[i]->data, sizeof(value_t) * a[i]->rows * a[i]->cols);
        datap += a[i]->rows * a[i]->cols;
    }
    return matrix_autorelease(r);
}

// autorelease function
// a:n*lと、b:n*hをn*(l+h)行列に連結。bが右。
matrix_t* matrix_cols_connect(matrix_t* a, matrix_t* b)
{
    if (a->rows != b->rows) {
        assert(0);
    }

	matrix_t* r = matrix_alloc(a->rows, a->cols + b->cols);
    int i;
    for (i = 0; i < a->rows; i++) 
	    memcpy(r->data + (a->cols + b->cols) * i, a->data + a->cols * i, sizeof(value_t) * a->cols);
    for (i = 0; i < a->rows; i++) 
	    memcpy(r->data + a->cols + (a->cols + b->cols) * i, b->data + b->cols * i, sizeof(value_t) * b->cols);

    return matrix_autorelease(r);
}

// autorelease function
// a:n*lと、b:n*hをn*(l+h)行列に連結。bが右。
matrix_t* matrix_multcols_connect(matrix_t** a, int matrix_num)
{
    int i, cyc;
    int tobecol = 0;
    for (i = 0; i < matrix_num - 1; i++) {
        if (a[i]->rows != a[i+1]->rows) {
            assert(0);
        }
        tobecol += a[i]->cols;
    }
    tobecol += a[matrix_num - 1]->cols;

	matrix_t* r = matrix_alloc(a[0]->rows, tobecol);
    int offset = 0;
    for (cyc = 0; cyc < matrix_num; cyc++) {
        for (i = 0; i < a[0]->rows; i++) 
	        memcpy(r->data + tobecol * i + offset, a[cyc]->data + a[cyc]->cols * i, sizeof(value_t) * a[cyc]->cols);
        offset += a[cyc]->cols;
    }

    return matrix_autorelease(r);
}

// autorelease function
matrix_t* matrix_operate_plus(matrix_t* a, matrix_t* b)
{
    return matrix_linear_comb(1.0, a, 1.0, b);
}

// autorelease function
matrix_t* matrix_operate_minus(matrix_t* a, matrix_t* b)
{
    return matrix_linear_comb(1.0, a, -1.0, b);
}

// autorelease function
matrix_t* matrix_linear_comb(value_t aval, matrix_t* a, value_t bval, matrix_t* b)
{
    if (a->cols != b->cols || a->rows != b->rows ) {
        assert(0);
    }
	matrix_t* r = matrix_alloc(a->rows, a->cols);

    int i;

	for (i = 0; i < a->rows * a->cols; i++) {
		r->data[i] = a->data[i] * aval + b->data[i] * bval;
	}
    return matrix_autorelease(r);
}

// autorelease function
matrix_t* matrix_scolar_product(value_t val, matrix_t* a)
{
	matrix_t* r = matrix_alloc(a->rows, a->cols);
	int n;
    int i;

	n = a->rows * a->cols;
	for (i = 0; i < n; i++) {
		r->data[i] = a->data[i] * val;
	}
    return matrix_autorelease(r);
}

// autorelease function
matrix_t* matrix_product(matrix_t* a, matrix_t* b)
{
	int r, c, j;

	if (a->cols != b->rows)
		return NULL;

	matrix_t* product = matrix_alloc(a->rows, b->cols);
	for (r = 0; r < product->rows; r++) {
		for (c = 0; c < product->cols; c++) {
			value_t sum = 0;
			for (j = 0; j < a->cols; j++) {
				MATRIX_ASSERT(a, r, j);
				MATRIX_ASSERT(a, j, c);
				sum += ELEMENT(a, r, j) * ELEMENT(b, j, c);
			}
			MATRIX_ASSERT(product, r, c);
			ELEMENT(product, r, c) = sum;
		}
	}
    return matrix_autorelease(product);
}

// autorelease function
// 転置
matrix_t* matrix_transpose(matrix_t* a)
{
	int r, c;

	matrix_t* t = matrix_alloc(a->cols, a->rows);
	for (r = 0; r < t->rows; r++) {
		for (c = 0; c < t->cols; c++) {
			MATRIX_ASSERT(t, r, c);
			MATRIX_ASSERT(a, c, r);
			ELEMENT(t, r, c) = ELEMENT(a, c, r);
		}
	}
	return matrix_autorelease(t);
}

void matrix_fill_zero(matrix_t* mat)
{
    int i, j;
    for (i = 0; i < mat->rows; i++) 
        for (j = 0; j < mat->cols; j++) 
            ELEMENT(mat, i, j) = 0.0;
}

// 行列式
value_t matrix_det(matrix_t* m)
{
	if (m->rows != m->cols) {
		assert(0);	// not a square matrix
	}
	if (m->rows == 2) {
		MATRIX_ASSERT(m, 1, 1);
		return ELEMENT(m, 0, 0) * ELEMENT(m, 1, 1) - ELEMENT(m, 0, 1) * ELEMENT(m, 1, 0);
	}
	if (m->rows == 3) {
		MATRIX_ASSERT(m, 2, 2);
		return \
			ELEMENT(m, 0, 0) * ELEMENT(m, 1, 1) * ELEMENT(m, 2, 2) \
			+ ELEMENT(m, 0, 1) * ELEMENT(m, 1, 2) * ELEMENT(m, 2, 0) \
			+ ELEMENT(m, 0, 2) * ELEMENT(m, 1, 0) * ELEMENT(m, 2, 1) \
			- ELEMENT(m, 0, 2) * ELEMENT(m, 1, 1) * ELEMENT(m, 2, 0) \
			- ELEMENT(m, 0, 1) * ELEMENT(m, 1, 0) * ELEMENT(m, 2, 2) \
			- ELEMENT(m, 0, 0) * ELEMENT(m, 1, 2) * ELEMENT(m, 2, 1);
			
	}
	assert(0);	// other dimensions are not not supported
}

// a:n*1 b:n*n c:n*1として、a^T b cの二次形式を計算する。
value_t matrix_quadratic(matrix_t* a, matrix_t* b, matrix_t* c)
{
    if (!(a->cols == 1 && c->cols == 1 && a->rows == b->cols && b->cols == b->rows && b->rows == c->rows)) {
        assert(0);
    }

    int i, j;
    value_t sum = 0.0;
    for (i = 0; i < b->cols; i++) 
        for (j = 0; j < b->cols; j++) 
            sum += ELEMENT(a, i, 0) * ELEMENT(c, j, 0) * ELEMENT(b, i, j);

    return sum;
}

// autorelease function
matrix_t* matrix_pseudo_inverse(matrix_t* m)
{
    if (m->rows == m->cols) {
        return matrix_inverse(m);
    } else if (m->rows > m->cols) {
        matrix_begin();
        // (A^T A)^{-1} A^T
        matrix_t* ret = matrix_product(matrix_inverse(matrix_product(matrix_transpose(m), m)),matrix_transpose(m));
        matrix_retain(ret);
        matrix_end();

        return matrix_autorelease(ret);
    } else {
        matrix_begin();
        // A^T (A A^T)^{-1} 
        matrix_t* ret = matrix_product(matrix_transpose(m), matrix_inverse(matrix_product(m, matrix_transpose(m))));
        matrix_retain(ret);
        matrix_end();

        return matrix_autorelease(ret);
    }
}

// non autorelease function
// maのia行目にaをかけた値と、mbのib行目にbをかけた値の和を、mdestのidestに代入
void matrix_row_linear_comb(float a, matrix_t* ma, int ia, float b, matrix_t* mb, int ib, matrix_t* mdest, int idest)
{
    if (ma->cols != mb->cols) 
        assert(0);

    int i;
	for (i=0;i<ma->cols;i++) 
		ELEMENT(mdest, idest, i) = a * ELEMENT(ma, ia, i) + b * ELEMENT(mb, ib, i);
}

// non autorelease function
void matrix_copy_row(matrix_t* msrc, int isrc, matrix_t* mdest, int idest)
{
    if (msrc->cols != mdest->cols) 
        assert(0);

	int i;
	for (i = 0; i < msrc->cols;i++) 
		ELEMENT(mdest, idest, i) = ELEMENT(msrc, isrc, i);
}

// autorelease function
matrix_t* matrix_inverse(matrix_t* m)
{
	if (m->rows != m->cols) {
        assert(0); // not a square matrix
    }
	
	int i, j;
	int maxi;
	int nrc;
	nrc = m->rows;
    matrix_t* minv = matrix_alloc(nrc, nrc);
    matrix_t* vtmp = matrix_alloc(1, nrc);

	float tmp;
	
    matrix_begin();
	matrix_t* mtmp = matrix_copy(m);
	for (i = 0; i < nrc * nrc; i++) {
		if (i / nrc == i % nrc) 
            ELEMENT(minv,i/nrc,i%nrc) = 1.0f;
		else
            ELEMENT(minv,i/nrc,i%nrc) = 0.0f;
	}

	if (mtmp == NULL || minv == NULL) 
        return NULL;
	
	for (i=0; i<nrc; i++) {
		maxi = i;
		for (j=i+1; j<nrc; j++) {
			if(ABS(ELEMENT(mtmp,maxi,i)) < ABS(ELEMENT(mtmp,j,i)))
				maxi = j;
		}
		if (ELEMENT(mtmp,maxi,i) == 0.0f) {
			printf("input matrix is invalid!\n");
			break;
		}
		
		// 行入れ替え
		matrix_copy_row(mtmp, i, vtmp, 0);
		matrix_copy_row(mtmp, maxi, mtmp, i);
		matrix_copy_row(vtmp, 0, mtmp, maxi);
		
		// 行入れ替え
		matrix_copy_row(minv, i, vtmp, 0);
		matrix_copy_row(minv, maxi, minv, i);
		matrix_copy_row(vtmp, 0, minv, maxi);
 
		tmp = ELEMENT(mtmp,i,i);

		for (j=0; j<nrc; j++) {
			// j:列番号
			ELEMENT(mtmp, i, j) = ELEMENT(mtmp, i, j)/tmp;
			ELEMENT(minv, i, j) = ELEMENT(minv, i, j)/tmp;
		}

		for (j=0; j<nrc; j++) {
			// j:行番号
			if	(i!=j) {
				tmp = ELEMENT(mtmp,j,i);
                matrix_row_linear_comb(-tmp, mtmp, i, 1.0f, mtmp, j, mtmp, j);
				matrix_row_linear_comb(-tmp, minv, i, 1.0f, minv, j, minv, j);
			}
		}
	}
	
    matrix_end();
	matrix_free(vtmp);

	return matrix_autorelease(minv);
}

void matrix_free(matrix_t* a)
{
	free(a->data);
	free(a);
}

void matrix_release(matrix_t* a)
{
	a->retainCount--;
	if (a->retainCount == 0)
		matrix_free(a);
}

void matrix_retain(matrix_t* a)
{
    a->retainCount++;
}

releasepool_t* new_node()
{
	return (releasepool_t *)malloc(sizeof(releasepool_t));
}

void free_node(releasepool_t* p)
{
	free(p);
}

void matrix_begin()
{
	releasepool_t* mark = new_node();
	mark->object = &marker;
	mark->next = pool;
	pool = mark;
}

void matrix_end()
{
	releasepool_t* nextp;

	while (pool->object != &marker) {
		nextp = pool->next;
		matrix_release(pool->object);	
		free_node(pool);
		pool = nextp;
	}
	nextp = pool->next;
	free_node(pool);
	pool = nextp;
}

matrix_t* matrix_autorelease(matrix_t* p)
{
	releasepool_t* node = new_node();
	node->object = p;
	node->next = pool;
	pool = node;

    return p;
}

void matrix_print(matrix_t* p)
{
    int i, j;
    for (i = 0; i < p->rows; i++) {
        for (j = 0; j < p->cols; j++) 
            fprintf(stdout, "%lf ", p->data[i * p->cols + j]);
        fprintf(stdout, "\n");
    }
}

void matrix_printerr(matrix_t* p)
{
    int i, j;
    for (i = 0; i < p->rows; i++) {
        for (j = 0; j < p->cols; j++) 
            fprintf(stderr, "%lf ", p->data[i * p->cols + j]);
        fprintf(stderr, "\n");
    }
}
