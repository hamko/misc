#include <stdlib.h>
#include <math.h>

// [0.0, +1.0]
float urandf_pfsys(void)
{
    return (float)rand() / RAND_MAX;
}

// [-1.0, +1.0]
float randf_pfsys(void)
{
    return (float)(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
}

// 
float box_muller(float mean, float sigma)
{
    float alpha, beta;
    if ((alpha = (float)rand() / RAND_MAX) == 0)
        return 0.0;
    beta = (float)rand() / RAND_MAX;

    return sigma * sqrt(-2 * log(alpha)) * cos(2 * M_PI * beta) + mean;
}


