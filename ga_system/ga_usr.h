#ifndef __GA_USR_H_INCLUDED__
#define __GA_USR_H_INCLUDED__

#include "params.h"
#include "ga_sys.h"

typedef struct _mgene_t {
    float pt;
    float x;
} mgene_t;

extern void mutate(ga_sys_t* gasys);
extern void init_parent(ga_sys_t* gasys);
extern void update_pt(ga_sys_t* gasys, int index);
extern void regist_ops(ga_sys_t* gasys);

#endif
