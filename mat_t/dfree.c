#ifdef PRINT_POINTER

#include <stdlib.h>
#include "robolib_matrix.h"

int dfree(mat_t* m)
{
    free(m);
    fprintf(stderr, "free : %p\n", m);
    return 0;
}

void* dalloc(int byte)
{
    void* p = malloc(byte);
    fprintf(stderr, "alloc: %p\n", p);
    return p;
}
#endif
