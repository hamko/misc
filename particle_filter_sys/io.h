#ifndef __IO_H_INCLUDED__
#define __IO_H_INCLUDED__

#define MAX_DATA 5000
#define MAX_PHOTOSENS 10

extern void input(void);
extern void output(void);
extern double gettimeofday_sec(void);

extern float io_mdats[MAX_DATA][MAX_PHOTOSENS];
extern int cyc;

#endif
