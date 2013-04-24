// xˆ k = Axˆ_k–1 + B_u_k
// Pk = AP_k – 1A^T + Q
// K_k = P_kH^T(HP_kH^T + R)^–1
// xˆ k = xˆ k + K_k(z_k -Hxˆ k)
// P_k (I – K_kH)P_k

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "robolib_matrix.h"
#include "matrix.h"
#include "rsys.h"
#include "newkfsys.h"

#define MDIM 1  // Measure Dimension
#define SDIM 2  // State Dimension
#define CDIM 1  // Controller Dimension
#define X_NOISE 0.03
#define DT 0.01 // [s]
#define SIM_CYCLE 1000

//val[0]: t
//val[1]: x
//val[2]: v
double f1(double *val)
{
    return val[2];
}

double f2(double *val)
{
    return -val[1];
}

double BoxMuller(double sigma, double mean)
{
    double alpha, beta;
    if ((alpha = (double)rand() / RAND_MAX) == 0)
        return 0.0;
    beta = (double)rand() / RAND_MAX;

    double bm = sigma * sqrt(-2 * log(alpha)) * cos(2 * M_PI * beta) + mean;
    return sigma * sqrt(-2 * log(alpha)) * cos(2 * M_PI * beta) + mean;
}

extern releasepool_t* pool;
int main(void)
{
    double m = 1.0; // [kg]
    double k = 1.0; // [N/m]
    double v = 0.05; // [m]
//    double u = 0.001; // [N]
    double u = 0.1; // [N]

    // 実おもりのシミュレーションに必要な変数の初期化
    // 実状態ベクトル初期化
    mat_t* xreal = robolib_column_vector_alloc(SDIM);   // 実状態ベクトル
    robolib_matrix_set(xreal, 0, 0, 1.0); 
    robolib_matrix_set(xreal, 1, 0, 0.0);

    // シミュレート用の構造体を定義
    rungesys_t rsys;
    rsysfunc_t funcarr[SDIM];
    funcarr[0] = f1;
    funcarr[1] = f2;
    double initialarr[SDIM+1]; // 時間の分だけ一つ多い
    initialarr[0] = 0.0; // [s]
    initialarr[1] = 1.0; // [m]
    initialarr[2] = 0.0; // [m/s]
    rsys_init(&rsys, SDIM, funcarr, initialarr, DT);



///////////////////////////////////
    // カルマンフィルタのために必要な変数の初期化
    kfsys_t kfsys;
    matrix_begin();
    // 以下kfsysがそもそも定常だと信じてる行列の初期化
    matrix_t* H = matrix_matrix(MDIM, SDIM);        // 観測行列
    matrix_t* initX = matrix_matrix(SDIM, 1);   // 初期平均
    matrix_t* initP = matrix_matrix(SDIM, SDIM);    // 初期分散
    matrix_t* Q = matrix_matrix(SDIM, SDIM);    // システム分散共分散行列

    // 観測行列
    ELEMENT(H, 0, 0) = 1.0;
    ELEMENT(H, 0, 1) = 0.0;

    // 初期状態平均
    ELEMENT(initX, 0, 0) = 0.0; // どこから始まるのかすらわからない
    ELEMENT(initX, 1, 0) = 1.0;

    // 初期状態分散
    ELEMENT(initP, 0, 0) = 0.1; // 大きめに設定しないとKが速く収束してしまうとかいう噂
    ELEMENT(initP, 1, 0) = 0.1;
    ELEMENT(initP, 0, 1) = 0.1;
    ELEMENT(initP, 1, 1) = 0.1;

    // システム分散共分散
    ELEMENT(Q, 0, 0) = 0.0001;
    ELEMENT(Q, 1, 0) = 0.00001;
    ELEMENT(Q, 0, 1) = 0.00001;
    ELEMENT(Q, 1, 1) = 0.0001;

   
    // カルマンフィルタに必要な変数のセット
    kfsys_init(&kfsys, SDIM, MDIM, CDIM, 0.0, DT, H, initX, initP, Q);

    matrix_end();
///////////////////////////////////

    matrix_begin();

    matrix_t* R = matrix_matrix(MDIM, MDIM);    // 観測分散共分散行列
    matrix_t* A = matrix_matrix(SDIM, SDIM);    // 状態遷移行列
    matrix_t* B = matrix_matrix(SDIM, CDIM);    // 制御遷移行列
    matrix_t* U = matrix_matrix(CDIM, 1);   // 制御
    matrix_t* Z = matrix_matrix(MDIM, 1); // allocしまくらないようにZも外に出しておく。

    // 観測分散共分散行列
    ELEMENT(R, 0, 0) = 0.01;

    // 状態遷移
    ELEMENT(A, 0, 0) = 1.0 - 0.5 * DT * DT * k / m;
    ELEMENT(A, 1, 0) = -DT * k / m;
    ELEMENT(A, 0, 1) = DT;
    ELEMENT(A, 1, 1) = 1.0;

    // 制御遷移
    ELEMENT(B, 0, 0) = 0.0;
    ELEMENT(B, 1, 0) = 0.0;

    // 制御
    ELEMENT(U, 0, 0) = 0.0;
 
    // シミュレーションと同時にカルマンフィルタリング
    int num;
    for (num = 0; num < SIM_CYCLE; num++) {
        // 重りの位置のシミュレーション 
        double* rsys_x = rsys_get_state(&rsys);
        robolib_matrix_set(xreal, 0, 0, rsys_x[1]);
        robolib_matrix_set(xreal, 1, 0, rsys_x[2]);

        matrix_t* posX = kfsys_getX(&kfsys);

        //posXがnull
        printf("%lf %lf %lf %lf %lf\n", robolib_matrix_get(xreal, 0, 0), robolib_matrix_get(xreal, 1, 0), ELEMENT(Z, 0, 0), ELEMENT(posX, 0, 0), ELEMENT(posX, 1, 0));

        rsys_revise(&rsys);
        ELEMENT(Z, 0, 0) = rsys_x[1] + BoxMuller(X_NOISE, 0);
        kfsys_revise(&kfsys, A, B, U, Z, R);
    }

    matrix_end();

    robolib_matrix_free(xreal);

    rsys_free(&rsys);
    kfsys_free(&kfsys);
    printf("%p\n", pool);
}
