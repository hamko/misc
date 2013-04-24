#ifdef PRINT_POINTER

#ifndef __DFREE_H_INCLUDED__
#define __DFREE_H_INCLUDED__

#define free(a) dfree(a)
#define malloc(a) dalloc(a)

#endif

#endif
