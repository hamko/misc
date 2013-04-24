#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"
#include "matrix_test.h"

extern releasepool_t* pool;
int main(int argc, char** argv)
{
    printf("--------verify--------\n");
    verify_matrix();
    printf("----------------------\n");

    // autorelease関数をbeginとendにはさまないで使うのはNG
    // 全てのallocされた関数は
    // (1) free
    // (2) release
    // (3) begin-endの中でautorelease
    // のいずれかを行う必要がある。
    
    // 擬似逆行列演算がこれくらい簡単に記述できる。
    
    /* 3*2行列aを確保、成分ごとの代入 */
    matrix_t* a = matrix_alloc(3, 2);
    ELEMENT(a, 0, 0) = 1; ELEMENT(a, 0, 1) = 4;
    ELEMENT(a, 1, 0) = 2; ELEMENT(a, 1, 1) = 5;
    ELEMENT(a, 2, 0) = 3; ELEMENT(a, 2, 1) = 6;

    /* 擬似逆行列の演算 */
    // auto releaseモードに入る
    matrix_begin(); 
    // 擬似逆行列を求める。一行だけ！
    matrix_t* inva = matrix_product(matrix_inverse(matrix_product(matrix_transpose(a), a)), matrix_transpose(a));
    // 擬似逆行列の表示 (invaはmatrix_endで解放されるので、begin-end内で。)
    matrix_print(inva);
    // release poolを開放する
    matrix_end();

    /* ちなみに擬似逆行列を求める関数は内部に組んだので、それを使うこともできる。 */
    // auto releaseモードに入る
    matrix_begin(); 
    // 擬似逆行列を求める関数を呼ぶ。
    matrix_t* inva_simple = matrix_pseudo_inverse(a);
    // 擬似逆行列の表示 (invaはmatrix_endで解放されるので、begin-end内で。)
    matrix_print(inva_simple);
    // release poolを開放する
    matrix_end();

    
    // これはautorelease対象でないので、しっかり自分でfree。
    // リテインカウントを実装してあるので、理解できればreleaseの方が高性能。
    //matrix_free(a);
    matrix_release(a);
 
    return 0;
}
