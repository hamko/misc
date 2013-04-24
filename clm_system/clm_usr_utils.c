#include <stdlib.h>

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
