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
#include "rsys.h"
#include "kfsys.h"

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

    // カルマンフィルタのために必要な変数の初期化
    kfsys_t kfsys;
    // 以下kfsysがそもそも定常だと信じてる行列の初期化
    mat_t* H = robolib_matrix_alloc(MDIM, SDIM);        // 観測行列
    mat_t* initX = robolib_column_vector_alloc(SDIM);   // 初期平均
    mat_t* initP = robolib_matrix_alloc(SDIM, SDIM);    // 初期分散
    mat_t* Q = robolib_matrix_alloc(SDIM, SDIM);    // システム分散共分散行列
    mat_t* R = robolib_matrix_alloc(MDIM, MDIM);    // 観測分散共分散行列
    mat_t* A = robolib_matrix_alloc(SDIM, SDIM);    // 状態遷移行列
    mat_t* B = robolib_matrix_alloc(SDIM, CDIM);    // 制御遷移行列
    mat_t* U = robolib_column_vector_alloc(CDIM);   // 制御

    // 観測行列
    robolib_matrix_set(H, 0, 0, 1.0);
    robolib_matrix_set(H, 0, 1, 0.0);

    // 初期状態平均
    robolib_matrix_set(initX, 0, 0, 0.0); // どこから始まるのかすらわからない
    robolib_matrix_set(initX, 1, 0, 1.0);

    // 初期状態分散
    robolib_matrix_set(initP, 0, 0, 0.1); // 大きめに設定しないとKが速く収束してしまうとかいう噂
    robolib_matrix_set(initP, 1, 0, 0.1);
    robolib_matrix_set(initP, 0, 1, 0.1);
    robolib_matrix_set(initP, 1, 1, 0.1);

    // システム分散共分散
    robolib_matrix_set(Q, 0, 0, 0.0001);
    robolib_matrix_set(Q, 1, 0, 0.00001);
    robolib_matrix_set(Q, 0, 1, 0.00001);
    robolib_matrix_set(Q, 1, 1, 0.0001);

    // 観測分散共分散行列
    robolib_matrix_set(R, 0, 0, 0.01);

    // 状態遷移
    robolib_matrix_set(A, 0, 0, 1.0 - 0.5 * DT * DT * k / m);
    robolib_matrix_set(A, 1, 0, -DT * k / m);
    robolib_matrix_set(A, 0, 1, DT);
    robolib_matrix_set(A, 1, 1, 1.0);

    // 制御遷移
    robolib_matrix_set(B, 0, 0, 0.0);
    robolib_matrix_set(B, 1, 0, 0.0);

    // 制御
    robolib_matrix_set(U, 0, 0, 0.0);
    
    // カルマンフィルタに必要な変数のセット
    kfsys_init(&kfsys, SDIM, MDIM, CDIM, 0.0, DT, H, initX, initP, Q);

    // allocしまくらないようにZも外に出しておく。
    mat_t* Z = robolib_column_vector_alloc(MDIM);

    // シミュレーションと同時にカルマンフィルタリング
    int num;
    for (num = 0; num < SIM_CYCLE; num++) {
        // 重りの位置のシミュレーション 
        double* rsys_x = rsys_get_state(&rsys);
        robolib_matrix_set(xreal, 0, 0, rsys_x[1]);
        robolib_matrix_set(xreal, 1, 0, rsys_x[2]);

        mat_t* posX = kfsys_getX(&kfsys);

        //posXがnull
        printf("%lf %lf %lf %lf %lf\n", robolib_matrix_get(xreal, 0, 0), robolib_matrix_get(xreal, 1, 0), robolib_matrix_get(Z, 0, 0), robolib_matrix_get(posX, 0, 0), robolib_matrix_get(posX, 1, 0));

        rsys_revise(&rsys);
        robolib_matrix_set(Z, 0, 0, rsys_x[1] + BoxMuller(X_NOISE, 0));
        kfsys_revise(&kfsys, A, B, U, Z, R);
    }

    robolib_matrix_free(H);
    robolib_matrix_free(initX);
    robolib_matrix_free(initP);
    robolib_matrix_free(Q);
    robolib_matrix_free(R);
    robolib_matrix_free(A);
    robolib_matrix_free(B);
    robolib_matrix_free(U);

    robolib_matrix_free(xreal);

    robolib_matrix_free(Z);
    kfsys_free(&kfsys);
}
