#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "clm_sys.h"

// オペレータの登録
void change_op(clm_sys_t* clmsys, void (*func)(clm_sys_t*, int))
{
    clmsys->operate = func;
}

// オペレータの登録
void change_parent_num(clm_sys_t* clmsys, int parent_num)
{
    clmsys->parent_num = parent_num;
}

void update_parent(clm_sys_t* clmsys)
{
    int j;
    for (j = 0; j < clmsys->parent_num; j++)
        clmsys->update_pt(clmsys, j);
//    sort_parent(clmsys);
}

void revise_parent(clm_sys_t* clmsys)
{
    int j;
    for (j = 0; j < clmsys->parent_num; j++)
        clmsys->operate(clmsys, j);
    clmsys->cycle++;
}

static void swap_gene(gene_t **a, gene_t **b)
{
    gene_t* c = *a;
    *a = *b;
    *b = c;
}

void sort_parent(clm_sys_t* clmsys)
{
    int i, j;
    update_parent(clmsys);

    for (i = 0; i < clmsys->parent_num - 1; i++) 
        for (j = 0; j < clmsys->parent_num - 1 - i; j++) 
            if (clmsys->parent[j]->pt < clmsys->parent[j+1]->pt) 
                swap_gene(&clmsys->parent[j], &clmsys->parent[j+1]);
}

void init_clm_sys(clm_sys_t* clmsys, void (*init_parent)(clm_sys_t*), void (*update_pt)(clm_sys_t*, int), void (*operate)(clm_sys_t*, int), int gene_bitlength, int parent_num)
{
    int i;
    srand((unsigned)time(NULL));

    clmsys->cycle = 0;
    clmsys->max_parent_num = parent_num;
    clmsys->parent_num = parent_num;

    clmsys->init_parent = init_parent;
    clmsys->update_pt = update_pt;

    // parentの数に応じてalloc
    clmsys->parent = malloc(clmsys->max_parent_num * sizeof(gene_t*));
    for (i = 0; i < clmsys->max_parent_num; i++)
        clmsys->parent[i] = malloc(gene_bitlength);
    clmsys->init_parent(clmsys);

    // オペレータの初期化
    clmsys->operate = operate;
    
//    update_parent(clmsys);
}

void free_clm_sys(clm_sys_t* clmsys)
{
    int i;
    free(clmsys->parent);
    for (i = 0; i < clmsys->max_parent_num; i++)
        free(clmsys->parent[i]);
}
