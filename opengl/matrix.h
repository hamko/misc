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

// pの所有権をリリースプールに移して、リリースプールにポインタを追加する
extern matrix_t* matrix_autorelease(matrix_t* p);
// リリースプールにマーカーを追加する
extern void matrix_begin();
// リリースプールに所有権が写されている行列を、マーカーまでリリースする
extern void matrix_end();

// 行列mのr行c列を返す
// このマクロは右辺値にも左辺値にもできる
#define ELEMENT(m, r, c)	((m)->data[(r) * (m)->cols + (c)])

// aがr行c列のまともな行列なのかを確認する
// MATRIX_DEBUGがdefineされていなければ、無視される
extern void matrix_assert(matrix_t* a, int r, int c);
#define MATRIX_DEBUG
#undef MATRIX_DEBUG
#if MATRIX_DEBUG
#define MATRIX_ASSERT(m, r, c) matrix_assert(m, r, c)
#else
#define MATRIX_ASSERT(m, r, c) 
#endif

/*********************************************************************/
// 行列の確保・所有・解放に関する関数群
/*********************************************************************/
/* 行列の確保 */
// !余程のことがなければ、下のmatrix_matrixを使うように。
// autorelease さ れ て い な い rows行cols列行列の確保
extern matrix_t* matrix_alloc(int rows, int cols);
// autoreleaseされたrows行cols列行列の確保
extern matrix_t* matrix_matrix(int rows, int cols);
// autoreleaseされたdim次単位行列の確保
extern matrix_t* matrix_unit(int dim);

/* 行列の保有・解放 */
// 行列のリテイン
extern void matrix_retain(matrix_t* a);
// !余程のことがなければ、下のmatrix_releaseを使うように。
// 行列の強制フリー
extern void matrix_free(matrix_t* a);
// 行列のリリース
extern void matrix_release(matrix_t* a);


/*********************************************************************/
// a == bに対応する関数群
/*********************************************************************/
#define COMPARE_PRECISION (1.0e-10)
// aとbの差の各要素の絶対値がprecisionに収まっている場合は1, そうでなければ0を返す
extern int matrix_compare_with_precision(matrix_t* a, matrix_t* b, value_t precision);
// aとbの差の各要素の絶対値がCOMPARE_PRECISIONに収まっている場合は1, そうでなければ0を返す
extern int matrix_compare(matrix_t* a, matrix_t* b);

/*********************************************************************/
// 様々な演算を提供する関数群。全てautoreleaseされた行列型ポインタが返る。
// 以下「行列」は、内部でmallocされた行列型ポインタを指す。
/*********************************************************************/
// aと同じ要素数の行列を新たにmatrix_matrixして返す
extern matrix_t* matrix_copy(matrix_t* a);
// 3*1ベクトルaの外積演算子行列を返す
extern matrix_t* matrix_exterior(matrix_t* a);

/* 行列連結関数 */
// aとbを縦に連結した行列を返す
extern matrix_t* matrix_rows_connect(matrix_t* a, matrix_t* b);
// aとbを横に連結した行列を返す
extern matrix_t* matrix_cols_connect(matrix_t* a, matrix_t* b);
// a[0]からa[matrix_num-1]までを縦に連結した行列を返す
extern matrix_t* matrix_multcols_connect(matrix_t** a, int matrix_num);
// a[0]からa[matrix_num-1]までを横に連結した行列を返す
extern matrix_t* matrix_multrows_connect(matrix_t** a, int matrix_num);

/* 行列演算関数 */
// aの要素に-1をかけた行列を返す
extern matrix_t* matrix_minus(matrix_t* a);
// aval * a + bval * bの行列を返す
extern matrix_t* matrix_linear_comb(value_t aval, matrix_t* a, value_t bval, matrix_t* b);
// a + bの行列を返す
extern matrix_t* matrix_operate_plus(matrix_t* a, matrix_t* b);
// a - bの行列を返す
extern matrix_t* matrix_operate_minus(matrix_t* a, matrix_t* b);
// val * aの行列を返す
extern matrix_t* matrix_scolar_product(value_t val, matrix_t* a);
// a * bの行列を返す
extern matrix_t* matrix_product(matrix_t* a, matrix_t* b);

/* 転置*/
// a^Tの転置行列を返す
extern matrix_t* matrix_transpose(matrix_t* a);

/* 逆行列 */
// aの逆行列を返す
extern matrix_t* matrix_inverse(matrix_t* a);
// aの最小二乗系擬似逆行列を返す
extern matrix_t* matrix_pseudo_inverse(matrix_t* m);

/*********************************************************************/
// 便利関数
/*********************************************************************/
/* matの各要素を0で埋める */
extern void matrix_fill_zero(matrix_t* mat);
/* 二次形式 a^T b cを計算する */
extern value_t matrix_quadratic(matrix_t* a, matrix_t* b, matrix_t* c);
/* 三次以下の行列mに対して行列式を計算する */
extern value_t matrix_det(matrix_t* m);

/*********************************************************************/
// 表示関数
/*********************************************************************/
/* 行列をprintfする */
extern void matrix_print(matrix_t* p);
/* 行列をstderrにfprintfする */
extern void matrix_printerr(matrix_t* p);

#endif
