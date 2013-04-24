#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "pf_sys.h"
#include "pf_usr.h"
#include "params.h"
#include "io.h"

int main(void)
{
    input();

    pfsys_t pfsys;
    
    init_pfsys(&pfsys, PARTICLE_NUM, PFSYS_DAT_NUM, linetracking_init_func, linetracking_likelyhood_func, linetracking_predict_func);
    int i, j;
    for (i = 0; i < cyc; i++) {
        float mdat[MAX_PHOTOSENS];
        for (j = 0; j < MDAT_NUM; j++)
            mdat[j] = io_mdats[i][j];
        revise_pfsys(&pfsys, mdat);
        float meanp[PFSYS_DAT_NUM] = {};
        float unipp[PFSYS_DAT_NUM] = {};
        float nmaxp[PFSYS_DAT_NUM] = {};
        get_particle_mean_pfsys(meanp, &pfsys);
        get_particle_unipolar_pfsys(unipp, &pfsys);
        get_particle_nmax_pfsys(5, nmaxp, &pfsys);
        printf("%f %f %f\n", meanp[0], unipp[0], nmaxp[0]);
    }

    output();

    return 0;
}
