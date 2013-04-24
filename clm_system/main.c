#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "clm_sys.h"
#include "clm_usr.h"
#include "params.h"
#include "io.h"

int main(void)
{
    input();
    double starttime = gettimeofday_sec();

    clm_sys_t maxsearch;
    init_clm_sys(&maxsearch, init_parent, update_pt, operator_, sizeof(mgene_t), 10);

    int i;
    for (i = 0; i < 10000; i++) {
        revise_parent(&maxsearch);
        update_parent(&maxsearch);
        printf("%f ", ((mgene_t*)(maxsearch.parent[0]))->x);
        printf("%f\n", ((mgene_t*)(maxsearch.parent[1]))->x);
    }

    // ここから仕上げ
    // 今のところの解をソートして
    sort_parent(&maxsearch);
    // 処理を精密モードに切り替えて
    change_op(&maxsearch, operator2_);
    // 処理する遺伝子を2個に限定する
    change_parent_num(&maxsearch, 2);
    for (; i < 12000; i++) {
        revise_parent(&maxsearch);
        update_parent(&maxsearch);
        printf("%f\n", ((mgene_t*)(maxsearch.parent[0]))->x);
        printf("%f\n", ((mgene_t*)(maxsearch.parent[1]))->x);
    }

    printf("time: %f\n", gettimeofday_sec() - starttime);
    output();

    free_clm_sys(&maxsearch);

    return 0;
}
