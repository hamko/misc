#ifndef __PF_USR_H_INCLUDED__
#define __PF_USR_H_INCLUDED__

#include "params.h"
#include "pf_sys.h"

extern float linetracking_likelyhood_func(float* pdat, float* mdat);
extern void linetracking_predict_func(float* pdat);
extern void linetracking_init_func(pfsys_t* pfsys);

#endif
