#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "clm_sys.h"
#include "clm_usr.h"
#include "clm_usr_utils.h"

float get_perfval(float x)
{
    return 32 - abs(x - 32);
}

/*****************************************/
/*                 mutate                */
/*****************************************/
// argminだけ更新し、ptは更新しない
void operator_(clm_sys_t* clmsys, int index) 
{
    mgene_t* pg = (mgene_t*)clmsys->parent[index];
    if (get_perfval(pg->x+0.001) < get_perfval(pg->x-0.001))
        pg->x -= 0.001;
    else
        pg->x += 0.001;
}

// argminだけ更新し、ptは更新しない
void operator2_(clm_sys_t* clmsys, int index) 
{
    mgene_t* pg = (mgene_t*)clmsys->parent[index];
    if (get_perfval(pg->x+0.00001) < get_perfval(pg->x-0.00001))
        pg->x -= 0.00001;
    else
        pg->x += 0.00001;
}


// argminだけ更新し、ptは更新しない
void init_parent(clm_sys_t* clmsys)
{
    int i;
    for (i = 0; i < clmsys->parent_num; i++) {
        mgene_t* pg = (mgene_t*)clmsys->parent[i];
        pg->x = 32 + frand() * 10;
    }
}

// ptだけ更新し、argminは更新しない
void update_pt(clm_sys_t* clmsys, int index)
{
    mgene_t* pg = (mgene_t*)clmsys->parent[index];
    pg->pt = get_perfval(pg->x);
}
