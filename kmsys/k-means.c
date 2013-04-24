#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "k-means.h"

int32_t dist_func_1norm(int32_t* dat1, int32_t* dat2, int16_t cnum)
{
    int i;
    int32_t sum = 0;
    for (i = 0; i < cnum; i++)
        sum += ABS(dat1[i] - dat2[i]);

    return sum;
}

int32_t dist_func_2norm(int32_t* dat1, int32_t* dat2, int16_t cnum)
{
    int i;
    int32_t sum = 0;
    for (i = 0; i < cnum; i++)
        sum += (dat1[i] - dat2[i]) * (dat1[i] - dat2[i]);

    return (int32_t)sqrt(sum);
}


void init_kmsys(kmsys_t* kmsys, int snum, int32_t (*dist_func)(int32_t* dat1, int32_t* dat2, int16_t cnum))
{
    int i, j;

    kmsys->cnum = 0;
    kmsys->dnum = 0;
    kmsys->snum = snum;
    for (i = 0; i < MAX_KMSYS_DNUM; i++) {
        for (j = 0; j < MAX_KMSYS_SNUM; j++) {
            (kmsys->dats[i]).dat[j] = 0;
        }
        (kmsys->dats[i]).var = -1;
    }
    for (i = 0; i < MAX_KMSYS_SNUM; i++) {
        for (j = 0; j < MAX_KMSYS_SNUM; j++) {
            (kmsys->clusters[i]).dat[j] = 0;
        }
    }
    kmsys->dist_func = dist_func;
}

void add_kmsys_dat(kmsys_t* kmsys, int32_t dat[MAX_KMSYS_DSIZE], int8_t var)
{
    int i;
    for (i = 0; i < kmsys->cnum; i++)
        (kmsys->dats[kmsys->dnum]).dat[i] = dat[i];
    (kmsys->dats[kmsys->dnum]).var = var;

    kmsys->dnum++;
}

void add_kmsys_cluster(kmsys_t* kmsys, int32_t dat[MAX_KMSYS_DSIZE])
{
    int i;
    for (i = 0; i < MAX_KMSYS_CNUM; i++)
        (kmsys->clusters[kmsys->cnum]).dat[i] = dat[i];

    kmsys->cnum++;
}

void add_kmsys_dat_string(kmsys_t* kmsys, char* dat, int8_t var)
{
    char* datp = strdup(dat);

    int i;
    for (i = 0, datp = strtok(datp, " "); datp != NULL; i++, datp = strtok(NULL, " ")) 
        (kmsys->dats[kmsys->dnum]).dat[i] = atoi(datp);
    (kmsys->dats[kmsys->dnum]).var = var;

    kmsys->dnum++;

    free(datp);
}

void add_kmsys_cluster_string(kmsys_t* kmsys, char* dat)
{
    char* datp = strdup(dat);

    int i;
    for (i = 0, datp = strtok(datp, " "); datp != NULL; i++, datp = strtok(NULL, " ")) 
        (kmsys->clusters[kmsys->cnum]).dat[i] = atoi(datp);

    kmsys->cnum++;

    free(datp);

    /*
    int i;
    for (i = 0; i < MAX_KMSYS_CNUM; i++)
        (kmsys->clusters[kmsys->cnum]).dat[i] = dat[i];

    kmsys->cnum++;
    */
}

void revise_kmsys(kmsys_t* kmsys)
{
    int i, j;
    
    // calculate center of cluster
    int cl_num[MAX_KMSYS_CNUM] = {};
    int cl_dat[MAX_KMSYS_CNUM][MAX_KMSYS_SNUM] = {};
    for (i = 0; i < kmsys->dnum; i++) {
        cl_num[(kmsys->dats[i]).var]++;
        for (j = 0; j < kmsys->snum; j++) 
            cl_dat[(kmsys->dats[i]).var][j] += (kmsys->dats[i]).dat[j];
    }
    for (i = 0; i < kmsys->cnum; i++) 
        if (cl_num[i]) 
            for (j = 0; j < kmsys->snum; j++) 
                (kmsys->clusters[i]).dat[j] = cl_dat[i][j] / cl_num[i];
    
    // revise variation of data
    for (i = 0; i < kmsys->dnum; i++) {
        int32_t mindist = INT_MAX;
        int32_t mindistj = -1;
        for (j = 0; j < kmsys->cnum; j++) {
            int dist = kmsys->dist_func((kmsys->dats[i]).dat, (kmsys->clusters[j]).dat, kmsys->cnum);
            if (mindist > dist) {
                mindist = dist;
                mindistj = j;
            }
        }
        (kmsys->dats[i]).var = mindistj;
    }
}

void print_kmsys_clusters(kmsys_t* kmsys)
{
    int i, j;

    for (i = 0; i < kmsys->cnum; i++) {
        for (j = 0; j < kmsys->snum; j++) 
            printf("%d ", (kmsys->clusters[i]).dat[j]);
        printf("\n");
    }
}

void print_kmsys_dats(kmsys_t* kmsys)
{
    int i, j;

    for (i = 0; i < kmsys->dnum; i++) {
        for (j = 0; j < kmsys->snum; j++) 
            printf("%d ", (kmsys->dats[i]).dat[j]);
        printf(": %d", (kmsys->dats[i]).var);
        printf("\n");
    }
}

