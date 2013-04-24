#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "pf_sys.h"
#include "pf_sys_util.h"

static void resample_particle(pfsys_t* pfsys)
{
    int i;
    float tmpparticle[MAX_PNUM * MAX_DNUM];
    float roulette_arr[MAX_PNUM];

    // copy
    memcpy(tmpparticle, pfsys->particle, sizeof(float) * pfsys->pnum * pfsys->dnum);
      
    // roullette array
    roulette_arr[0] = pfsys->weight[0];
    for (i = 1; i < pfsys->pnum; i++)
        roulette_arr[i] = roulette_arr[i - 1] + pfsys->weight[i];
    
    // recopy
    for (i = 0; i < pfsys->pnum; i++) {
        int n = 0;
        float wsum = urandf_pfsys() * roulette_arr[pfsys->pnum - 1];
        while (roulette_arr[++n] < wsum);
        memcpy(&(pfsys->particle[i]), &(tmpparticle[n]), sizeof(float) * pfsys->dnum);
        pfsys->weight[i] = NOT_MEASURED_WEIGHT;
    }
}

static void calc_particle_weight(pfsys_t* pfsys, float* mdat)
{
    int i;
    
    float wsum = 0.0f;
    for (i = 0; i < pfsys->pnum; i++) { 
        pfsys->weight[i] = pfsys->likelyhood_func(&(pfsys->particle[i*pfsys->dnum]), mdat);
        wsum += pfsys->weight[i];
    }
    for (i = 0; i < pfsys->pnum; i++) 
        pfsys->weight[i] = pfsys->weight[i] / wsum;
}

void init_pfsys(pfsys_t* pfsys, int particle_num, int dat_num, void (*init_particles)(pfsys_t* pfsys), float (*likelyhood_func)(float* pdat, float* mdat), void (*predict_func)(float* pdat))
{
    int i;

    pfsys->pnum = particle_num;
    pfsys->dnum = dat_num;
    pfsys->likelyhood_func = likelyhood_func;
    pfsys->predict_func = predict_func;
    for (i = 0; i < pfsys->pnum; i++)
        pfsys->weight[i] = NOT_MEASURED_WEIGHT;

    init_particles(pfsys);
}

void revise_pfsys(pfsys_t* pfsys, float* mdat)
{
    int i;

    resample_particle(pfsys);
    for (i = 0; i < pfsys->pnum; i++)
        pfsys->predict_func(&(pfsys->particle[i*pfsys->dnum]));
    calc_particle_weight(pfsys, mdat);
}

/*************************************/
// パーティクルの代表を選ぶ関数
/*************************************/
// 全パーティクルの相加ベクトル平均
// mean_particle[MAX_DNUM];
void get_particle_mean_pfsys(float* mean_particle, pfsys_t* pfsys)
{
    int i, j;
    
    for (i = 0; i < pfsys->pnum; i++)
        for (j = 0; j < pfsys->dnum; j++)
            mean_particle[j] += pfsys->particle[i*pfsys->dnum+j];
    for (j = 0; j < pfsys->dnum; j++)
        mean_particle[j] /= pfsys->pnum;
}

// 全パーティクルの尤度重み付け平均
// weightが正しく1に正規化されていることを利用。
// unipolay_particle[MAX_DNUM];
void get_particle_unipolar_pfsys(float* unipolar_particle, pfsys_t* pfsys)
{
    int i, j;
    
    for (i = 0; i < pfsys->pnum; i++)
        for (j = 0; j < pfsys->dnum; j++)
            unipolar_particle[j] += pfsys->particle[i*pfsys->dnum+j] * pfsys->weight[i];
}

// パーティクルの交換
static void swap_particle(int field_num, float* a, float* b)
{
    int i;
    float tmpparticle[MAX_DNUM];

    for (i = 0; i < field_num; i++)
        tmpparticle[i] = a[i];
    for (i = 0; i < field_num; i++)
        a[i] = b[i];
    for (i = 0; i < field_num; i++)
        b[i] = tmpparticle[i];
}

// 重みの交換
static void swap_weight(float* a, float* b)
{
    float c = *a;
    *a = *b;
    *b = c;
}

// パーティクルの尤度の高いものからnum個の相加ベクトル平均
// nmax_particle[MAX_DNUM];
void get_particle_nmax_pfsys(int num, float* nmax_particle, pfsys_t* pfsys)
{
    int i, j;
    
    if (num > pfsys->pnum) {
        printf("num must be smaller than pnum.\n");
        exit(1);
    }

    // copy
    float tmpparticle[MAX_PNUM * MAX_DNUM];
    float tmpweight[MAX_PNUM];
    memcpy(tmpparticle, pfsys->particle, sizeof(float) * pfsys->pnum * pfsys->dnum);
    memcpy(tmpweight, pfsys->weight, sizeof(float) * pfsys->pnum);
      
    // sort
    for (i = 0; i < pfsys->pnum - 1; i++) {
        for (j = 0; j < pfsys->pnum - i - 1; j++) {
            if (tmpweight[j] < tmpweight[j+1]) {
                swap_particle(pfsys->dnum, &tmpparticle[j*pfsys->dnum], &tmpparticle[(j+1)*pfsys->dnum]);
                swap_weight(&tmpweight[j], &tmpweight[j+1]);
            }
        }
    }

    for (i = 0; i < num; i++)
        for (j = 0; j < pfsys->dnum; j++)
            nmax_particle[j] += tmpparticle[i*pfsys->dnum+j];
    for (j = 0; j < pfsys->dnum; j++)
        nmax_particle[j] /= num;
}

