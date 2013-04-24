#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "ga_sys.h"

// オペレータの登録。始めから割り当てていく。開いている所のoperateはNULL。
void regist_op(ga_sys_t* gasys, void (*func)(ga_sys_t*), int8_t cnum, int weight)
{
    int i = 0;
    while (gasys->op[i].operate != NULL)
        i++;

    gasys->op[i].operate = func;
    gasys->op[i].cnum = cnum;
    gasys->op[i].weight = weight;
}

// ルーレット方式でparentのインデックスを選ぶ。
int16_t roulette_gn(ga_sys_t* gasys)
{
    int i;
    //ここのptsumが0未満になると0除算 randにかけている値は適当
    if (gasys->ptsum <= 0) {
        fprintf(stderr, "ptsum <= 0, aborted.");
        exit(1);
    }
	float r = gasys->ptsum * ((float)rand() / RAND_MAX);
    float sum = 0.0; 

    // あらかじめそこまでの和を持っておけばlog nでいける
    i = 0;
    while (1) {
        if ((sum += gasys->parent[gasys->order[i]]->pt) > r)
            break;
        i++;
    }

    return gasys->order[i];
}

// aとbを入れ替える。
static void swap(int16_t *a, int16_t *b)
{
    int16_t temp = *a;
    *a = *b;
    *b = temp;
}

// int16_tはorderの型に合わせている
/* qsortの使い方が分からないからとりあえずコメントアウト
static int compare_int(const void* a, const void* b)
{
    return parent[*(int16_t*)b]->pt - parent[*(int16_t*)a]->pt;
}
*/

// parentのptを降順にソートしてorderを更新
static void sort_parent(ga_sys_t* gasys)
{
    int i, j;
    for (i = 0; i < gasys->parent_num; i++)
        gasys->order[i] = i;
    gasys->order[gasys->parent_num - 1] = 0;
    gasys->order[0] = gasys->parent_num - 1;

    // gasys->parent_numが小さい時はバブルソートの方が早い
    for (i = 0; i < gasys->parent_num - 1; i++)
        for (j = 0; j < gasys->parent_num - i - 1; j++)
            if (gasys->parent[gasys->order[j]]->pt < gasys->parent[gasys->order[j+1]]->pt)
                swap(&(gasys->order[j]), &(gasys->order[j+1]));

    // TODO How to use qsort when I must pass a gasys?
    // qsort(order, gasys->parent_num, sizeof(int16_t), compare_int);
}

// gene_tのfromをtoにコピーする
static void gncpy(ga_sys_t* gasys, gene_t *to, gene_t *from)
{
    memcpy(to, from, gasys->gene_bitlength);
}

// parentのエリートをchildrenに移す
static void select_elite(ga_sys_t* gasys)
{
    for (; gasys->cn < gasys->elite_num; (gasys->cn)++)
        gncpy(gasys, gasys->children[gasys->cn], gasys->parent[gasys->order[gasys->cn]]);
}

// operatorのルーレット選択
static void roulette_op(ga_sys_t* gasys, operator_t *_op)
{
    int i;
    int sum = 0;

    if (gasys->wsum == 0) {
        fprintf(stderr, "gasys->wsum is 0. Aborted.\n");
        exit(1);
    }

    do {
        i = 0;
        while (1) {
            if ((sum += gasys->op[i].weight) > rand() % gasys->wsum)
                break;
            i++;
        }
    } while (gasys->cn + gasys->op[i].cnum > gasys->parent_num);

    _op->operate = gasys->op[i].operate;
    _op->cnum = gasys->op[i].cnum;
    _op->weight = gasys->op[i].weight;
}

// ルーレット方式でchildrenを埋める
static void alternate_generation(ga_sys_t* gasys)
{
    while (gasys->cn < gasys->parent_num) {
        operator_t _op;
        roulette_op(gasys, &_op);
        _op.operate(gasys);
        gasys->cn += _op.cnum;
    }
}

// childrenをparentに移す
static void copy_ctop(ga_sys_t* gasys)
{
    int i;
    for (i = 0; i < gasys->parent_num; i++)
        gncpy(gasys, gasys->parent[i], gasys->children[i]);
}

static void update_parent(ga_sys_t* gasys)
{
    int i, j;
    for (j = 0; j < gasys->parent_num; j++)
        gasys->update_pt(gasys, j);
    sort_parent(gasys);

    gasys->ptsum = 0;
    for (i = 0; i < gasys->parent_num; i++) {
        float point = gasys->parent[i]->pt;
        if (point <= 0) {
            fprintf(stderr, "pt < 0 (i = %d, pt = %f)\nexit.\n", i, point);
            exit(1);
        }
        gasys->ptsum += point;
    }
    if (gasys->ptsum <= 0) {
        fprintf(stderr, "ptsum <= 0 (ptsum = %f)\nexit.\n", gasys->ptsum);
        exit(1);
    }
}

void init_ga_sys(ga_sys_t* gasys, void (*init_parent)(ga_sys_t*), void (*update_pt)(ga_sys_t*, int), void (*register_ops)(ga_sys_t*), int16_t parent_num, int16_t elite_num, int16_t gene_bitlength)
{
    int i;
    srand((unsigned)time(NULL));

    // 各種ユーザ定義の関数を保存
    gasys->init_parent = init_parent;
    gasys->update_pt = update_pt;
    gasys->register_ops = register_ops;

    // 遺伝子の長さを保存
    gasys->gene_bitlength = gene_bitlength;

    // parentの数に応じてalloc
    gasys->parent_num = parent_num;
    gasys->parent = malloc(parent_num * sizeof(gene_t*));
    for (i = 0; i < parent_num; i++)
        gasys->parent[i] = malloc(gene_bitlength);
    gasys->init_parent(gasys);

    // childrenの数に応じてalloc
    gasys->children = malloc(parent_num * sizeof(gene_t*));
    for (i = 0; i < parent_num; i++)
        gasys->children[i] = malloc(gene_bitlength);
    
    // orderの領域確保
    gasys->order = malloc(parent_num * sizeof(int16_t));

    // エリートをとる数を保存
    // エリート主義をとらないならば、ここは0にする
    gasys->elite_num = elite_num;

    // オペレータの初期化
    for (i = 0; i < MAX_OPERATOR_NUM; i++)
        gasys->op[i].operate = NULL;
    gasys->register_ops(gasys);
    
    // オペレータの重み総和を保存
    gasys->wsum = 0;
    i = 0;
    while (gasys->op[i].operate != NULL)
        gasys->wsum += gasys->op[i++].weight;

    update_parent(gasys);
}

// GENERATION_NUM世代parentを遺伝計算する。
void alternate_generations(ga_sys_t* gasys)
{
    gasys->cn = 0;

    select_elite(gasys);
    alternate_generation(gasys);
    copy_ctop(gasys);
    update_parent(gasys);
}
