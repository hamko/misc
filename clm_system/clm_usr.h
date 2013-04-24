#ifndef __GA_USR_H_INCLUDED__
#define __GA_USR_H_INCLUDED__

#include "params.h"
#include "clm_sys.h"

typedef struct _mgene_t {
    float pt;
    float x;
} mgene_t;

extern void mutate(clm_sys_t* clmsys);
extern void init_parent(clm_sys_t* clmsys);
extern void update_pt(clm_sys_t* clmsys, int index);
extern void regist_ops(clm_sys_t* clmsys);
extern void operator_(clm_sys_t* clmsys, int index);
extern void operator2_(clm_sys_t* clmsys, int index);

#endif
