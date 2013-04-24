#ifndef __GA_USR_UTILS_H_INCLUDED__
#define __GA_USR_UTILS_H_INCLUDED__

extern void get_mgene_parents(ga_sys_t *gasys, mgene_t **pg, int pnum);
extern void get_mgene_children(ga_sys_t *gasys, mgene_t **cg, int cnum);

extern float frand(void);
extern float frandp(void);

#endif
