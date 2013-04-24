#ifndef __PF_SYS_H_INCLUDED__
#define __PF_SYS_H_INCLUDED__

#include "stdint.h"

#define NOT_MEASURED_WEIGHT (1.0f)
#define MAX_PNUM 1000
#define MAX_DNUM 20
#define PFINDEX(pfsys,pfindex,dn) (((pfsys)->dnum)*(pfindex)+(dn))
#define PFELEM(pfsys,pfindex,dn) (pfsys->particle[((pfsys)->dnum)*(pfindex)+(dn)])

#undef MICON
//#define MICON

struct _pfsys_t;
typedef struct _pfsys_t {
    int16_t pnum;   // the number of particles
    int16_t dnum;   // the dim of a particle vector
    float (*likelyhood_func)(float* pdat, float* mdat);
    void (*predict_func)(float* pdat);
    float weight[MAX_PNUM];   // weight[i]がi番目のパーティクルの重み
    float particle[MAX_PNUM * MAX_DNUM]; // particle[i*dnum+j]がi番目のパーティクルのj成分
} pfsys_t;

extern void init_pfsys(pfsys_t* pfsys, int particle_num, int dat_num, void (*init_particles)(pfsys_t* pfsys), float (*likelyhood_func)(float*, float*), void predict_func(float*));

extern void revise_pfsys(pfsys_t* pfsys, float* mdat);

extern void get_particle_mean_pfsys(float* mean_particle, pfsys_t* pfsys);

extern void get_particle_unipolar_pfsys(float* unipolar_particle, pfsys_t* pfsys);

extern void get_particle_nmax_pfsys(int num, float* nmax_particle, pfsys_t* pfsys);

extern int get_mdat_num(pfsys_t* pfsys);

extern void print_particles(pfsys_t* pfsys);

#endif
