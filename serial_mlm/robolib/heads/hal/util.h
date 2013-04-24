/*!
  \file   util.h
  \author TAKISAWA, Jun'ichi
  \date   Fri Feb  8 20:00:41 2008
  
  \brief  あると便利な物。
  
*/

#ifndef _UTIL_H_
#define _UTIL_H_

#ifndef NULL
#define NULL (void *)(0)
#endif

//#define printf rs_printf

short rs_printf(const char *format, ...);

#ifdef _DEBUG_
#define dprintf(...) rs_printf("#D:"__VA_ARGS__)
#endif //_DEBUG_

#ifndef _DEBUG_
#define dprintf(...) 
#endif //!_DEBUG_

typedef enum
{
  FALSE = 0,
  TRUE = 1
} BOOLEAN;

typedef unsigned char uchar;

#define _UNUSED_ __attribute__ ((unused))
#define UNUSED_VARIABLE(x) (void)(x)

#endif //_UTIL_H_
