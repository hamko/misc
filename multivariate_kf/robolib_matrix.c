/*
 *  robolib_matrix.c
 *  Untitled
 *
 *  Created by 谷本 啓 on 11/04/09.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *	
 */
#include "robolib_matrix.h"
#include <stdio.h>
#include <stdlib.h>
#include "dfree.h"

#ifndef myABS
#define myABS(a) ((a)>0?(a):(-a))
#endif

#ifndef predDEBUG
#define predDEBUG
#endif

// エラーログ等を吐くかどうか
//#define ROBOLIB_MATRIX_PRINT

/***************************************/
/* 配列操作								*/
/***************************************/

void robolib_arr_linear_comb(double a, double* va, double b, double* vb, double* vc, int dim)
{
	int i;
	for (i=0;i<dim;i++) {
		vc[i]=a*va[i]+b*vb[i];
	}
}

double robolib_arr_average(double* a, int dim)
{
	int i;
	double tmp;
	tmp=0;
	for (i=0;i<dim;i++) {
		tmp+=a[i];
	}
	return tmp/dim;
}

/***************************************/
/* 行列操作・演算用関数                    */
/***************************************/
/* rows行 cols列の行列を動的に確保する */
/* この関数で確保した行列は必ずrobolib_matrix_free関数で解放しなければならない */
/**
 * robolib_matrix_alloc:
 * @rows: desired number of rows in matrix.
 * @cols: desired number of columns.
 *
 * Returns: pointer to a newly allocated robolib_matrix, or NULL
 * on failure.  Note that the actual data storage is not
 * initialized.
 */

mat_t* robolib_matrix_alloc (int rows, int cols)
{
	mat_t* m;
	
//    if (rows < 0 || cols < 0) {
		// 0行or0列はvalをnullにしてstructure全体の領域のみalloc
    if (rows < 0 || cols < 0) {
		 /*err_log_begin 
#ifdef robolib_matrix_log_true
		FILE* fp;
		fp = fopen("log/error.txt","a");
		char msg[] = "ERROR: input rows or cols is invalid!\n";
		fprintf(fp,"%s input rows and cols:(%d*%d)", 
				msg, rows, cols);
#endif
		err_log_end */
		return NULL;
    }
	
//	if (!rows || !cols){
//		return NULL;
//	}
//	

	m = malloc(sizeof *m);
    if (m == NULL) {
		//		set_robolib_matrix_err(E_ALLOC);
		return NULL;
    }
	
	if (rows*cols) {
		m->val = malloc(rows * cols * sizeof *m->val);
		if (m->val == NULL) {
			//		set_robolib_matrix_err(E_ALLOC);
			free(m);
			return NULL; // malloc失敗
		}
		m->rows = rows;
		m->cols = cols;
	} else {
		//0行or0列のval以外allocが呼ばれたとき
		m->val = NULL;
		m->rows = 0;
		m->cols = 0;
	}
	

	//    m->t1 = m->t2 = 0;
	
    return m;
}

/* ハコだけ再利用する
 * 
 *
 */

mat_t* robolib_matrix_realloc(mat_t* m, int rows, int cols)
{
		
	if (rows < 0 || cols < 0) {
		/* err_log_begin 
#ifdef robolib_matrix_log_true 
		FILE* fp;
		fp = fopen("log/error.txt","a");
		char msg[] = "ERROR: input rows or cols is invalid!\n";
		fprintf(fp,"%s\n input rows and cols:(%d*%d)", 
				msg, rows, cols);
#endif
		 err_log_end */
		return NULL;
    }
	if (rows*cols) {
		if (m->val!=NULL) {
			free(m->val);
		}
		m->val = malloc(rows * cols * sizeof *m->val);
		if (m->val == NULL) {
			free(m);
			return NULL; // malloc失敗
		}
		m->rows = rows;
		m->cols = cols;
	} else {
		//0行or0列のval以外allocが呼ばれたとき
		m->val = NULL;
		m->rows = 0;
		m->cols = 0;
	}
	
	return m;
	
}


/**
 * robolib_matrix_free:
 * @m: matrix to be freed.
 *
 * Frees the allocated storage in @m, then @m itself.
 */

void robolib_matrix_free (mat_t* m)
{
    if (robolib_matrix_is_null(m)) return;
	
    if (m->val != NULL) {
		free(m->val);
    }
	
    free(m);

}

mat_t* robolib_matrix_copy(const mat_t* ma, mat_t* mb)
{
	if (robolib_is_null_matrix(ma)) {
		return NULL;
	}
	
	if (robolib_matrix_is_null(mb)){
		mb = robolib_matrix_alloc(ma->rows,ma->cols);
	} else if (robolib_matrix_is_empty(mb)||!robolib_matrix_is_same_size(ma,mb)) {
		robolib_matrix_realloc(mb, ma->rows, ma->cols);
	}

	memcpy(mb->val, ma->val, ma->rows * ma->cols * sizeof *ma->val);
	return mb;
}

mat_t* robolib_matrix_new_zero(int rows, int cols, mat_t* m){
	if (rows<=0||cols<=0) return NULL;
	
	if (robolib_matrix_is_null(m)){
		robolib_matrix_free(m);

		m = robolib_matrix_alloc(rows,cols);
	}
	
	mat_t* mhat;
	mhat = robolib_matrix_alloc(rows,cols);
	#ifdef predDEBUG
	if (robolib_matrix_is_null(mhat))
		printf("matrix allocation error");
	#endif
	int i;
	for (i=0;i<rows*cols;i++) {
		robolib_matrix_set(mhat,i/cols,i%cols,0);
	}

	robolib_matrix_copy(mhat, m);
	robolib_matrix_free(mhat);
	
	return m;
}


mat_t* robolib_matrix_new_unit(int nrc)
{
	if (nrc<=0) return NULL;
	
//	if (robolib_is_null_matrix(m) || m->rows!=m->cols) {
//		m = robolib_matrix_alloc(nrc,nrc);
//	} else if (nrc!=m->rows || nrc!=m->cols) {
	
//	if (robolib_is_null_matrix(m))
	// これチェックするとセグフォ起こることがある。
	// freeした後で0xffffffffになってた場合。
	//	robolib_matrix_free(m);
	// freeも駄目だった。これはminvを消すしかない.
	mat_t* m;
	m = robolib_matrix_alloc(nrc,nrc);
//	}
	
	mat_t* mhat;
	mhat = robolib_matrix_alloc(nrc, nrc);
	
	int i;
	for (i=0; i<nrc*nrc; i++) {
		if (i/nrc == i%nrc) robolib_matrix_set(mhat,i/nrc,i%nrc,1.0);
		else robolib_matrix_set(mhat,i/nrc,i%nrc,0.0);
	}
	
	robolib_matrix_copy(mhat, m);
	robolib_matrix_free(mhat);
	
	return m;
}

void robolib_matrix_scalar_product(double a, mat_t* m)
{
	int i;
	for (i=0; i<m->rows*m->cols; i++) {
		m->val[i] *= a;
	}
}

mat_t* robolib_matrix_trans(const mat_t* m, mat_t* mt)
{
	if (robolib_is_null_matrix(m)) {
		return NULL;
	}
	
	if (mt == NULL){
		mt = robolib_matrix_alloc(m->cols,m->rows);
	} else if (!robolib_matrix_is_empty(mt) && m->rows==mt->cols && m->cols==mt->rows) {
    } else {
		// 行列のサイズがおかしい
		mt = robolib_matrix_realloc(mt,m->cols,m->rows);
	}
	int i,j;
	
//	mat_t* mhat;
//	mhat = robolib_matrix_alloc(m->rows,m->cols);
	
	for(i=0;i<m->rows;i++){
		for(j=0;j<m->cols;j++){
			robolib_matrix_set(mt,j,i,robolib_matrix_get(m,i,j));
		}
	}
	
//	robolib_matrix_copy(mhat,mt);
//	robolib_matrix_free(mhat);
	
	return mt;
}

mat_t* robolib_matrix_linear_comb(double a, const mat_t* ma,
										    double b, const mat_t* mb, mat_t* mc)
{
	if (robolib_is_null_matrix(ma)||robolib_is_null_matrix(mb)){
		return NULL;
	}
	if (!robolib_matrix_is_same_size(ma,mb)) return NULL;
	
	if (robolib_matrix_is_null(mc)){
		mc = robolib_matrix_alloc(ma->rows,ma->cols);
	} else if (!robolib_matrix_is_empty(mc) && robolib_matrix_is_same_size(ma,mc)) {
    } else {
		mc = robolib_matrix_realloc(mc,ma->rows,ma->cols);
	}
		
	mat_t* mhat;
	mhat = robolib_matrix_alloc(ma->rows,ma->cols);
	int i;
	for (i=0; i< ma->rows*ma->cols; i++) {
		mhat->val[i] = a * ma->val[i] + b * mb->val[i];
	}
	robolib_matrix_copy(mhat, mc);
	robolib_matrix_free(mhat);

	return mc;
}

/* 
 * mc=ma*mb
 */
mat_t* robolib_matrix_product(const mat_t* ma, const mat_t* mb, mat_t* mc)
{
	if (robolib_is_null_matrix(ma) || robolib_is_null_matrix(mb) || ma->cols != mb->rows) {
		return NULL;
	}

	if (robolib_matrix_is_null(mc)) {
		mc = robolib_matrix_alloc(ma->rows,mb->cols);
	} else if (!robolib_matrix_is_empty(mc) && mc->rows == ma->rows && mc->cols == mb->cols) {
    } else {
		mc = robolib_matrix_realloc(mc,ma->rows,mb->cols);		
	}

	int i,j,k;
	
	for(i=0;i<ma->rows;i++){
		for(j=0;j<mb->cols;j++){
			double tmp = 0;
			for (k=0; k<ma->cols; k++) {
				tmp += robolib_matrix_get(ma,i,k) * robolib_matrix_get(mb,k,j);
			}
			robolib_matrix_set(mc,i,j,tmp);
		}
	}
	return mc;
}


/* n乗を求める */
/*void MultMatTimes(double** ma, double** mb, int n, int nrc)
{
	int i;
	for (i=0; i<n; i++) {
		MultMatMat(ma, ma, mb, nrc, nrc, nrc);
	}
}*/

/* __TODO__
 今のところminvを渡す意味がない。
 new_unitで再allocしてしまっている。
 
 引数: (m,NULL)
 戻り値: minv
 */

mat_t* robolib_matrix_inverse(mat_t* m, mat_t* minv)
{
	if (robolib_is_null_matrix(m)||m->rows!=m->cols) return NULL;
	
	int i,j;
	int maxi;
	int nrc;
	nrc = m->rows;
	mat_t* mtmp = robolib_matrix_alloc(nrc, nrc);
	mat_t* vtmp = robolib_vector_alloc(nrc);
	double tmp;
	
	robolib_matrix_copy(m, mtmp);
	minv = robolib_matrix_new_unit(nrc);
	if (mtmp==NULL || minv==NULL) return NULL;
	
	for (i=0; i<nrc; i++) {
//		maxi = 0; あれっこれ違…
		maxi = i;
		for (j=i+1; j<nrc; j++) {
			if(myABS(robolib_matrix_get(mtmp,maxi,i)) < myABS(robolib_matrix_get(mtmp,j,i)))
				maxi = j;
		}
		if (robolib_matrix_get(mtmp,maxi,i) == 0.0) {
			printf("input matrix is invalid!\n");
			break;
		}
		
		// 行入れ替え
		robolib_matrix_copy_row(mtmp, i, vtmp, 0);
		robolib_matrix_copy_row(mtmp, maxi, mtmp, i);
//		robolib_matrix_copy_row(vtmp, 0, mtmp, i);
		robolib_matrix_copy_row(vtmp, 0, mtmp, maxi);
		
		// 行入れ替え
		robolib_matrix_copy_row(minv, i, vtmp, 0);
		robolib_matrix_copy_row(minv, maxi, minv, i);
//		robolib_matrix_copy_row(vtmp, 0, minv, i);
		robolib_matrix_copy_row(vtmp, 0, minv, maxi);
 
		
		tmp = robolib_matrix_get(mtmp,i,i);

		for (j=0; j<nrc; j++) {
			// j:列番号
			//*(mtmp->val+i*mtmp->cols+j) /= tmp;
			//*(minv->val+i*mtmp->cols+j) /= tmp;
			robolib_matrix_set(mtmp, i, j,
							   robolib_matrix_get(mtmp, i, j)/tmp);
			robolib_matrix_set(minv, i, j,
							   robolib_matrix_get(minv, i, j)/tmp);
//			printf("%s : %d\n",__FILE__,__LINE__);
//			robolib_matrix_print(mtmp);
		}

		for (j=0; j<nrc; j++) {
			// j:行番号
			if	(i!=j) {
//				tmp = robolib_matrix_get(m,j,i);
				tmp = robolib_matrix_get(mtmp,j,i);
				robolib_matrix_row_linear_comb(-tmp, mtmp, i, 1.0, mtmp, j, mtmp, j);
				robolib_matrix_row_linear_comb(-tmp, minv, i, 1.0, minv, j, minv, j);
//				printf("%s : %d\n",__FILE__,__LINE__);
//				robolib_matrix_print(mtmp);
			}
		}
		
	}
	
	robolib_vector_free(vtmp);
	robolib_matrix_free(mtmp);
	
	return minv;
	
}

// 注意
// よくわかんないけどNULL渡す方がよいかも
mat_t* robolib_matrix_arr_average(mat_t** arrm, int dim, mat_t* mave)
{
	if (robolib_is_null_matrix(arrm[0])) return NULL;
//このifをつけるとcopy(mhat, mave)がうまくいかなくなるっぽい？
	//	if (robolib_is_null_matrix(mave)) {
	
	//robolib_matrix_new_zero(arrm[0]->rows,arrm[0]->cols,mave);
	// いや原因は↑か？
	mave = robolib_matrix_new_zero(arrm[0]->rows,arrm[0]->cols,NULL);
//	}
	mat_t* mhat;
	mhat = robolib_matrix_new_zero(arrm[0]->rows,arrm[0]->cols,NULL);
	int i,j;
	for (i=1;i<dim;i++) {
		if (!robolib_matrix_is_same_size(arrm[0],arrm[i])) 
			return NULL;
	}
	for (j=0;j<(mhat->rows * mhat->cols);j++){
		for (i=0;i<dim;i++) {
			mhat->val[j] += arrm[i]->val[j];
		}
		mhat->val[j] /= dim;
	}
	robolib_matrix_copy(mhat, mave);
	robolib_matrix_free(mhat);
	return mave;
}

/* 行列mの一部( (irow,icol)からrows*cols部分 )を新たな行列にコピーする */
/* (memo: mpartの大きさでrows,colsを指定するのもアリ…だがとりあえず分かり易さ重視) */
mat_t* robolib_matrix_copy_part(mat_t* m, int irow, int icol, int rows, int cols,
										 mat_t* mpart)
{
	if (robolib_is_null_matrix(m) ||
		m->rows < irow+rows || m->cols < icol+cols )
		return NULL;
	
	mat_t* mhat;
	if (robolib_matrix_is_null(mpart)){
		mpart = robolib_matrix_alloc(rows, cols);
	} else if (robolib_matrix_is_empty(mpart) || mpart->rows != rows || mpart->cols != cols) {
//		robolib_matrix_free(mpart);
		robolib_matrix_realloc(mpart,rows, cols);
	}
	mhat = robolib_matrix_alloc(rows, cols);
	int i,j;
	for (i=0;i<rows;i++) {
		for (j=0;j<cols;j++) {
			robolib_matrix_set(mhat, i, j,
							   robolib_matrix_get(m, irow+i, icol+j));
		}
	}
	robolib_matrix_copy(mhat, mpart);
	robolib_matrix_free(mhat);
	return mpart;
}

/* 行列全体を行列の一部( (irow,icol)から始まるm->rows*m->cols部分 )にコピーする */
/* mlargeがNullMatrixだと駄目 */
mat_t* robolib_matrix_copy_to_part(mat_t* m, mat_t* mlarge, int irow, int icol)
{
	if (robolib_is_null_matrix(m) || robolib_is_null_matrix(mlarge) ||
		mlarge->rows < irow+m->rows || mlarge->cols < icol+m->cols )
		return NULL;

	int i,j;
	for (i=0;i<m->rows;i++) {
		for (j=0;j<m->cols;j++) {
			robolib_matrix_set(mlarge, irow+i, icol+j,
							   robolib_matrix_get(m, i, j));
		}
	}

	return mlarge;
}

/* 行列の1行を他の行列にコピー */

mat_t* robolib_matrix_copy_row(mat_t* msrc, int isrc, mat_t* mdest, int idest)
{
	if (robolib_is_null_matrix(msrc)) return NULL;
	if (msrc->rows<=isrc) return NULL;
	
	int destrow;
	destrow = idest;
	
	// colsが違ったらdestをベクトルにする
	if (robolib_is_null_matrix(mdest) || mdest->cols!=msrc->cols) {
//		robolib_matrix_free(mdest);
		mdest = robolib_vector_realloc(mdest, msrc->cols);
		destrow = 0;
	}

	int i;
	for (i=0;i<msrc->cols;i++) {
		robolib_matrix_set(mdest, destrow, i,
						   robolib_matrix_get(msrc, isrc, i));
	}
	
	return mdest;
	
}

/* 1行と1行の線形和 */
mat_t* robolib_matrix_row_linear_comb(double a, mat_t* ma, int ia, double b, mat_t* mb, int ib, mat_t* mdest, int idest)
{
	if (robolib_is_null_matrix(ma)||robolib_is_null_matrix(mb)||ma->cols!=mb->cols)
		return NULL;
	
	int destrow;
	destrow = idest;
	if (robolib_is_null_matrix(mdest) || mdest->cols!=ma->cols){
//		robolib_matrix_free(mdest);
		mdest = robolib_vector_realloc(mdest, ma->cols);
		destrow = 0;
	}
	
	int i;
	for (i=0;i<ma->cols;i++) {
		robolib_matrix_set(mdest, destrow, i,
						   a * robolib_matrix_get(ma, ia, i)
						   + b * robolib_matrix_get(mb, ib, i));
	}
	
	return mdest;
	
}


void robolib_matrix_print(mat_t* m)
{
#ifdef ROBOLIB_MATRIX_PRINT
	if (robolib_is_null_matrix(m)){
		printf("given matrix is NULL \n");
		return;
	}

	printf("given matrix is:\n(%d*%d)\n",m->rows,m->cols);
	int i,j;
	for (i=0;i<m->rows;i++){
		for (j=0;j<m->cols;j++){
			printf("%f ",robolib_matrix_get(m,i,j));
		}
		printf("\n");
	}
	printf("\n");
#endif
}

void robolib_matrix_log(FILE *fp, mat_t* m)
{
	if (robolib_is_null_matrix(m)){
            fprintf(fp, "matrix is NULL \n");
		return;
	}
	
	fprintf(fp, "matrix(%d*%d):\n",m->rows,m->cols);
	int i,j;
	for (i=0;i<m->rows;i++){
		for (j=0;j<m->cols;j++){
                    fprintf(fp, "%f ",robolib_matrix_get(m,i,j));
		}
		fprintf(fp, "\n");
	}
	fprintf(fp, "\n");
}


