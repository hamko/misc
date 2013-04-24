#include "option.h"

#ifdef PRINT_POINTER

#ifndef __DFREE_H_INCLUDED__
#define __DFREE_H_INCLUDED__

#include "robolib_matrix.h"

extern int dfree(void* m);
extern void* dalloc(size_t byte);

#define free(a) dfree(a)
#define malloc(a) dalloc(a)

#endif

#endif
