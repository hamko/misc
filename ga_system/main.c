#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "ga_sys.h"
#include "ga_usr.h"
#include "params.h"
#include "io.h"

int main(void)
{
    input();
    double starttime = gettimeofday_sec();

    ga_sys_t maxsearch;
    
    init_ga_sys(&maxsearch, init_parent, update_pt, regist_ops, PARENT_NUM, ELITE_NUM, sizeof(mgene_t));
    int i;
    for (i = 0; i < GENERATION_NUM; i++) {
        alternate_generations(&maxsearch);
        printf("%f\n", ((mgene_t*)(maxsearch.parent[maxsearch.order[0]]))->x);
    }

    printf("time: %f\n", gettimeofday_sec() - starttime);
    output();

    return 0;
}
