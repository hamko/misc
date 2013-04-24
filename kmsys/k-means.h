#ifndef __K_MEAN_SYS_H_INCLUDED__
#define __K_MEAN_SYS_H_INCLUDED__

#include <stdint.h>

#define MAX_KMSYS_DNUM 1024
#define MAX_KMSYS_DSIZE 8
#define MAX_KMSYS_CNUM 16
#define MAX_KMSYS_SNUM 16

#define ABS(a) ((a)>0?(a):-(a))

// 各データとそのクラスタを保存
typedef struct _kmsys_dat_t {
    int32_t dat[MAX_KMSYS_SNUM];
    int8_t var; // variety
} kmsys_dat_t;

// 各クラスタに対する重心
typedef struct _kmsys_state_t {
    int32_t dat[MAX_KMSYS_SNUM];
} kmsys_state_t;

typedef struct _kmsys_t {
    int16_t snum; // state num
    int16_t cnum; // cluster num
    int32_t dnum; // dat num
    kmsys_dat_t dats[MAX_KMSYS_DNUM];
    kmsys_state_t clusters[MAX_KMSYS_SNUM];
    int32_t (*dist_func)(int32_t* dat1, int32_t* dat2, int16_t cnum);
} kmsys_t;

void print_kmsys_dats(kmsys_t* kmsys);
void print_kmsys_clusters(kmsys_t* kmsys);
void revise_kmsys(kmsys_t* kmsys);
void add_kmsys_cluster_string(kmsys_t* kmsys, char* dat);
void add_kmsys_dat_string(kmsys_t* kmsys, char* dat, int8_t var);
void add_kmsys_cluster(kmsys_t* kmsys, int32_t dat[MAX_KMSYS_DSIZE]);
void add_kmsys_dat(kmsys_t* kmsys, int32_t dat[MAX_KMSYS_DSIZE], int8_t var);
void init_kmsys(kmsys_t* kmsys, int snum, int32_t (*dist_func)(int32_t* dat1, int32_t* dat2, int16_t cnum));
int32_t dist_func_1norm(int32_t* dat1, int32_t* dat2, int16_t cnum);
int32_t dist_func_2norm(int32_t* dat1, int32_t* dat2, int16_t cnum);
#endif
