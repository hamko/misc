#ifndef __GA_SYS_H_INCLUDED__
#define __GA_SYS_H_INCLUDED__

#include "stdint.h"
#include "params.h"

typedef struct _gene_t {
    float pt;
    // 実際にはptの後にユーザ定義のデータ領域が確保されている
} gene_t;

typedef struct _clm_sys_t clm_sys_t;

struct _clm_sys_t {
    gene_t** parent;
    void (*operate)(struct _clm_sys_t* clmsys, int index);
    void (*init_parent)(struct _clm_sys_t* clmsys); // 始めのparentの設定
    void (*update_pt)(struct _clm_sys_t* clmsys, int index); // 始めのparentの設定
    int cycle;
    int max_parent_num;
    int parent_num;
};

extern void init_clm_sys(clm_sys_t* clmsys, void (*init_parent)(clm_sys_t*), void (*update_pt)(clm_sys_t*, int), void (*operate)(clm_sys_t*, int), int gene_bitlength, int parent_num);

void change_op(clm_sys_t* clmsys, void (*func)(clm_sys_t*, int));
void update_parent(clm_sys_t* clmsys);
void revise_parent(clm_sys_t* clmsys);
void free_clm_sys(clm_sys_t* clmsys);
void sort_parent(clm_sys_t* clmsys);
void change_parent_num(clm_sys_t* clmsys, int parent_num);

#endif
