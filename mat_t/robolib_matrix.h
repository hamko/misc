/*
 *  robolib_matrix.h
 *  Untitled
 *
 *  Created by 谷本 啓 on 11/04/09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

/**
 * robolib_matrix:
 * @rows: number of rows in matrix
 * @cols: number of columns
 * @val: flat array of double-precision values
 *
 * The basic robolib matrix type; #robolib_vector is an alias
 * that can be used for matrices with @rows or @cols = 1.
 */

/* ::memo::
	TODO
	・書き込み中のブロック処理(robolib_copy_to_part() )
 */

#include <string.h>
#include <stdio.h>

#ifndef __ROBOLIB_MATRIX_H__
#define __ROBOLIB_MATRIX_H__

/***************************************/
/* 行列定義								*/
/***************************************/

typedef struct mat_t_ {
	int rows;
    int cols;
    double *val;
} mat_t;

/***************************************/
/* 配列操作								*/
/***************************************/

#define robolib_memcpy_arr(a,b,dim) memcpy(a,b,dim * sizeof *a)

void robolib_arr_linear_comb(double a, double* va, double b, double* vb, double* vc, int dim);

double robolib_arr_average(double* a, int dim);

/***************************************/
/* 行列操作・演算用マクロ                    */
/***************************************/

/**
 * robolib_matrix_get:
 * @m: matrix.
 * @i: row.
 * @j: column.
 * 
 * Returns: the @i, @j element of @m.
 */

#define robolib_matrix_get(m,i,j) (m->val[(j)*m->rows+(i)])

/**
 * robolib_vector_get:
 * @v: vector.
 * @i: index.
 * 
 * Returns: element @i of @v.
 */

#define robolib_vector_get(v,i) (v->val[i])

/**
 * robolib_matrix_set:
 * @m: matrix.
 * @i: row.
 * @j: column.
 * @x: value to set.
 * 
 * Sets the @i, @j element of @m to @x.
 */

#define robolib_matrix_set(m,i,j,x) ((m)->val[(j)*(m)->rows+(i)]=x)

/**
 * robolib_vector_set:
 * @v: vector.
 * @i: index.
 * @x: value to set.
 * 
 * Sets element @i of @v to @x.
 */

#define robolib_vector_set(v,i,x) ((v)->val[i]=x)

/**
 * robolib_matrix_cols:
 * @m: matrix to query.
 * 
 * Returns: the number of columns in @m. 
 */

#define robolib_matrix_cols(m) ((m == NULL)? 0 : m->cols)

/**
 * robolib_matrix_rows:
 * @m: matrix to query.
 * 
 * Returns: the number of rows in @m. 
 */

#define robolib_matrix_rows(m) ((m == NULL)? 0 : m->rows)

/**
 * robolib_vector_get_length:
 * @v: vector to examine.
 * 
 * Returns: the length of vector @v (without regard to whether
 * it is a row or column vector).
 */

#define robolib_vector_get_length(v) ((v == NULL)? 0 : \
(v->cols == 1)? v->rows : \
(v->rows == 1)? v->cols : 0)

/**
 * robolib_vector_alloc:
 * @i: number of columns.
 *
 * Returns: a new #robolib_vector with @i columns.
 */

#define robolib_vector_alloc(i) robolib_matrix_alloc(1,(i))

/**
 * robolib_column_vector_alloc:
 * @i: number of rows.
 *
 * Returns: a new column robolib_vector with @i rows.
 */

#define robolib_column_vector_alloc(i) robolib_matrix_alloc((i),1)

#define robolib_vector_realloc(m,i) robolib_matrix_realloc((m), 1, (i))

#define robolib_column_vector_realloc(m,i) robolib_matrix_realloc((m), (i), 1)

/**
 * robolib_vector_free:
 * @v: %robolib_vector to free.
 *
 * Frees the vector @v and its associated storage.
 */

#define robolib_vector_free(v) robolib_matrix_free(v)

/**
 * robolib_matrix_is_scalar:
 * @m: matrix to test.
 *
 * Returns: 1 if @m is 1 x 1, else 0.
 */

#define robolib_matrix_is_scalar(m) ((m) != NULL && \
(m)->rows == 1 && \
(m)->cols == 1)

// なんか変なマトリックス。完全体ではないもの。
// ただしちゃんとallocされてるかどうかチェックできているわけではないので注意
#define robolib_is_null_matrix(m) ((m) == NULL || (m)->rows == 0 || (m)->cols == 0 || (m)->val==NULL)

//ハコもない
#define robolib_matrix_is_null(m) ((m) == NULL)
//ハコだけ定義されたmatrix
#define robolib_matrix_is_empty(m) ((m)->rows == 0 || (m)->cols == 0 || (m)->val==NULL)

#define robolib_matrix_is_same_size(ma,mb) ((ma)!=NULL && (mb)!=NULL && ma->rows == mb->rows && ma->cols == mb->cols)

/***************************************/
/* 行列操作・演算用関数                    */
/***************************************/

/* *注意*
 * alloc系(alloc,newが入ってるメソッド)は必ずfreeすること。
 * (作った行列の参照は予め引数で指定するか返り値を代入して後でfreeできるようにしておくこと。)
 * allocはただallocするだけ。newはNULL渡すとalloc、サイズがおかしいとfreeして再度alloc
 * ただし宣言しただけのポインタを渡すと変なアドレスを読みに行く可能性があるのと
 * そのポインタの指す場所以外にallocしてしまって見失うので、robolib_matrix_alloc(0,0)をして
 * matrix structureのみalloc。valをallocしないでおく。
 */

// Alloc系

/* rows行 cols列の行列を動的に確保する */
/* この関数で確保した行列は必ずrobolib_matrix_free関数で解放しなければならない */
mat_t* robolib_matrix_alloc(int rows, int cols);

/* matrixのハコの領域は維持してvalだけfreeのち再度alloc */
mat_t* robolib_matrix_realloc(mat_t* m, int rows, int cols);

/* 上のAllocMat関数で確保した行列のメモリを解放 */
void robolib_matrix_free(mat_t* m);

/* 行列maを(あればmbに)コピーし、mbを返す。無ければallocする。以下同様NULLならalloc。 */
/* mbを返すのは式中で使いたい時のため */
/* maとmbのサイズが違えばmbは一旦freeして再alloc */
mat_t* robolib_matrix_copy(mat_t* ma, mat_t* mb);

/* rows行cols列の零行列をallocして返す */
mat_t* robolib_matrix_new_zero(int rows, int cols, mat_t* m);

/* nrc行nrc列の正方行列を返す */
mat_t* robolib_matrix_new_unit(int nrc);

// 演算系

/* 行列mの各要素を定数a倍にする */
void robolib_matrix_scalar_product(double a, mat_t* m);

/* 行列mの転置行列mtを作る */
// 正しい行列ならそのまま、mtに変な行列だったらrealloc, allocしてない行列だったらalloc
mat_t* robolib_matrix_trans(mat_t* m, mat_t* mt);

/* 2つの行列maとmbの線形和 (a*ma+b*mb)を求め、結果をmcに入れる */
// 正しい行列ならそのまま、mtに変な行列だったらrealloc, allocしてない行列だったらalloc
mat_t* robolib_matrix_linear_comb(double a, mat_t* ma, double b, mat_t* mb, 
				mat_t* mc);

/* 上記を配列として実装 */
/* 上の方に記述 void robolib_arr_linear_comb(double a, double* va, double b, double* vb, double* vc, int dim)*/

/* 2つの行列maとmbの積 (ma*mb)を求め、結果をmcに入れる */
// 正しい行列ならそのまま、mtに変な行列だったらrealloc, allocしてない行列だったらalloc
mat_t* robolib_matrix_product(mat_t* ma, mat_t* mb, mat_t* mc);

/* ガウス・ジョルダン法でmの逆行列minvを求める */
mat_t* robolib_matrix_inverse(mat_t* m, mat_t* minv);

// ユーティリティ
/* robolib_matrixの配列の平均行列を求める */
mat_t* robolib_matrix_arr_average(mat_t** arrm, int dim, mat_t* mave);

/* 行列の一部( (irow,icol)からrows*cols部分 )を新たな行列にコピーする */
mat_t* robolib_matrix_copy_part(mat_t* m, int irow, int icol, int rows, int cols, mat_t* mpart);

/* 行列を行列の一部( (irow,icol)から始まるm->rows*m->cols部分 )にコピーする */
mat_t* robolib_matrix_copy_to_part(mat_t* m, mat_t* mlarge, int irow, int icol);

/* 行列の1行をコピー */

mat_t* robolib_matrix_copy_row(mat_t* msrc, int isrc, mat_t* mdest, int idest);

/* 行列の1列をコピー */
//mat_t* robolib_matrix_copy_column(mat_t* msrc, int icol, mat_t* vdest);

/* 1行と1行の線形和 */
mat_t* robolib_matrix_row_linear_comb(double a, mat_t* ma, int ia, double b, mat_t* mb, int ib, mat_t* mdest, int idest);

/***************************************/
/* ログ用									*/
/***************************************/

/* 標準出力にプリント */
void robolib_matrix_print(mat_t* m);

/* 同ファイルに出力 */
void robolib_matrix_log(FILE *fp, mat_t* m);

#endif
