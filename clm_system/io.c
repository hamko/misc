#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "io.h"

static double starttime;
double gettimeofday_sec(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec * 1e-6;
}

void input(void)
{
}

void output(void)
{
}
