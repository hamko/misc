#include "option.h"
#ifdef PRINT_POINTER

#include <stdlib.h>
#include "robolib_matrix.h"

void dfree(void* m)
{
    free(m);
    fprintf(stdout, "free : %p\n", m);
    fprintf(stderr, "free : %p\n", m);
}

void* dalloc(size_t byte)
{
    void* p = malloc(byte);
    fprintf(stdout, "alloc: %p\n", p);
    fprintf(stderr, "alloc: %p\n", p);
    return p;
}
#endif
