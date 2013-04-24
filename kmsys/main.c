#include <time.h>
#include <stdlib.h>
#include "k-means.h"

#define rand3 (rand()%3)

int main(void)
{
    srand((unsigned int)time(NULL));

    kmsys_t kmsys_c; // concrete
    kmsys_t* kmsys = &kmsys_c;

    init_kmsys(kmsys, 2, dist_func_1norm); // init_kmsys(kmsys_pointer, cnum);

    add_kmsys_cluster_string(kmsys, "50 50");
    add_kmsys_cluster_string(kmsys, "100 100");
    add_kmsys_cluster_string(kmsys, "250 250");

    add_kmsys_dat_string(kmsys, "1 1", rand3);
    add_kmsys_dat_string(kmsys, "-2 2", rand3);
    add_kmsys_dat_string(kmsys, "5 -5", rand3);
    add_kmsys_dat_string(kmsys, "101 101", rand3);
    add_kmsys_dat_string(kmsys, "99 99", rand3);
    add_kmsys_dat_string(kmsys, "200 210", rand3);
    add_kmsys_dat_string(kmsys, "190 200", rand3);
    
    int i;
    for (i = 0; i < 10; i++) {
        revise_kmsys(kmsys);
    }
        
    print_kmsys_clusters(kmsys);
    print_kmsys_dats(kmsys);

    return 0;
}
