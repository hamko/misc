#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "pf_sys.h"
#include "pf_sys_util.h"
#include "pf_usr.h"
#include "params.h"

void linetracking_init_func(pfsys_t* pfsys)
{
    int i;

    for (i = 0; i < PARTICLE_NUM; i++) {
        PFELEM(pfsys, i, 0) = (float)i * MDAT_NUM / PARTICLE_NUM;
//        PFELEM(pfsys, i, 0) = 1;
//        pfsys->particle[i*2] = (float)i * MDAT_NUM / PARTICLE_NUM;
        PFELEM(pfsys, i, 1) = 0;
    }
}

#define DIV_ROOT_TWO_PI 0.39894228f
static float get_gauss(float m, float v)
{
    return DIV_ROOT_TWO_PI * exp(-m*m/2/v/v) / v;
}

#define LINETRACKING_VARIANCE 1.5
//#define LINETRACKING_VARIANCE 0.1
float linetracking_likelyhood_func(float* pdat, float* mdat)
{
    int i;
    float mean = 0.0f;
    int active_num = 0;
    for (i = 0; i < MDAT_NUM; i++) {
        if (!mdat[i])
            continue;
        mean += i;
        active_num++;
    }
    mean /= active_num;
    
    float gauss = get_gauss(mean - pdat[0], LINETRACKING_VARIANCE);
#ifdef DEBUG_MODE
    printf("(gauss, %f, %f, %f)\n", gauss, mean, pdat[0]);
#endif
    if (!active_num)
        return 0.0001;
    else
        return gauss;
}

#define PREDICT_VARIANCE 0.05
void linetracking_predict_func(float* pdat)
{
//    pdat[0] += randf_pfsys() * PREDICT_VARIANCE;
    pdat[0] += randf_pfsys() * PREDICT_VARIANCE;
}
