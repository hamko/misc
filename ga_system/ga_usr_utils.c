#include <stdlib.h>
#include "ga_sys.h"
#include "ga_usr.h"
#include "params.h"

/*****************************************/
/* function to conbert gene_t to mgene_t */
/*****************************************/
void get_mgene_parents(ga_sys_t *gasys, mgene_t **pg, int pnum)
{
    int i;
    for (i = 0; i < pnum; i++) 
        pg[i] = (mgene_t*)gasys->parent[roulette_gn(gasys)];
}

void get_mgene_children(ga_sys_t *gasys, mgene_t **cg, int cnum)
{
    int i;
    for (i = 0; i < cnum; i++) 
        cg[i] = (mgene_t*)gasys->children[(gasys->cn)+i];
}

/*****************************************/
/*        function to get random         */
/*****************************************/
// float rand function, which returns value[from -1.0 to 1.0].
float frand(void)
{
    return (float)(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
}

// float rand function, which returns value[from 0.0 to 1.0].
float frandp(void)
{
    return (float)rand() / RAND_MAX ;
}
