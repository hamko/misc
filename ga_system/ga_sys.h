#ifndef __GA_SYS_H_INCLUDED__
#define __GA_SYS_H_INCLUDED__

#include "stdint.h"
#include "params.h"

typedef struct _gene_t {
    float pt;
    // 実際にはptの後にユーザ定義のデータ領域が確保されている
} gene_t;

struct _ga_sys_t;

// ユーザ定義のオペレータを保存する型。
typedef struct _operator_t {
    void (*operate)(struct _ga_sys_t* gasys);
    int weight; // ルーレット方式で使われる重み
    int cnum;   // 生成される子供の数
} operator_t;

typedef struct _ga_sys_t {
    gene_t** parent;
    gene_t** children;
    operator_t op[MAX_OPERATOR_NUM];
    void (*init_parent)(struct _ga_sys_t* gasys); // 始めのparentの設定
    void (*update_pt)(struct _ga_sys_t* gasys, int index); // 評価関数
    void (*register_ops)(struct _ga_sys_t* gasys); // regist_opを集めたもの
    int16_t* order; // parent[order[i]]番がparentでi+1番目に良い遺伝子となる
    int16_t parent_num; // parent, childrenでallocする個数
    int16_t elite_num; // parent, childrenでallocする個数
    int16_t cn; // オペレータはchildren[cn]からchildren[cn+cnum-1]までを埋める
    float ptsum; // parentのptの総和
    int16_t wsum; // opのweightの総和
    int16_t gene_bitlength; // ユーザ定義のデータ領域も含めた染色体の大きさ
} ga_sys_t;

/* gasysを初期化する */
/* init_parent      parentの初期化 */
/* update_pt        parentの評価 */
/* register_ops     operatorの登録 */
/* parent_num       parentの数 */
/* elite_num        エリートの数。エリート主義を取らないなら0 */
/* gene_bitlength   ユーザ定義遺伝子の型の大きさ。sizeof(usrgene_t) */
extern void init_ga_sys(ga_sys_t* gasys, void (*init_parent)(ga_sys_t*), void (*update_pt)(ga_sys_t*, int), void (*register_ops)(ga_sys_t*), int16_t parent_num, int16_t elite_num, int16_t gene_bitlength);

/* gasysに子供をcnum生むfuncを登録する */
/* 出現頻度はweightでルーレット方式 */
extern void regist_op(ga_sys_t* gasys, void (*func)(ga_sys_t*), int8_t cnum, int weight);

/* roulette方式で遺伝子を選び、選ばれたgasys->parentのインデックスを返す */
extern int16_t roulette_gn(ga_sys_t* gasys);

/* gasysの世代交代を行う */
extern void alternate_generations(ga_sys_t* gasys);

#endif
