#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "ga_sys.h"
#include "ga_usr.h"
#include "params.h"
#include "ga_usr_utils.h"

/*****************************************/
/*                 mutate                */
/*****************************************/
void mutate(ga_sys_t* gasys) 
{
    mgene_t* cg[MUTATE_CNUM]; get_mgene_children(gasys, cg, MUTATE_CNUM);
    mgene_t* pg[MUTATE_PNUM]; get_mgene_parents(gasys, pg, MUTATE_PNUM);

    cg[0]->x = pg[0]->x + frand()* 3;
}

void init_parent(ga_sys_t* gasys)
{
    int i;
    for (i = 0; i < gasys->parent_num; i++) {
        mgene_t* pg = (mgene_t*)gasys->parent[i];
        pg->x = 32 + frand() * 10;
    }
}

void update_pt(ga_sys_t* gasys, int index)
{
    mgene_t* pg = (mgene_t*)gasys->parent[index];
    pg->pt = 32 - abs(pg->x - 32);
}

void regist_ops(ga_sys_t* gasys)
{
    regist_op(gasys, mutate, MUTATE_CNUM, WEIGHT_OP_MUTATE);
}
