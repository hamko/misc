#include <stdlib.h>
#include <math.h>
#include "params.h"

// [0.0, +1.0]
float urandf_pfsys(void)
{
#ifdef MICON
#error "rand is not available"
    return -1;
#else 
    return (float)rand() / RAND_MAX;
#endif
}

// [-1.0, +1.0]
float randf_pfsys(void)
{
#ifdef MICON
#error "rand is not available"
    return -1;
#else 
    return (float)(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
#endif
}

