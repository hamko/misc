#include <omp.h>
#include <stdio.h>
#define N 5

int main ()
{
    int a[N], b[N], c[N], d[N];
    for (int i=0; i<N; i++) a[i] = b[i] = i;
#pragma omp parallel
    {
#pragma omp sections
        {
#pragma omp section
            {
                for (int i=0; i<N; i++){
                    c[i] = a[i] + b[i];
                    printf("Thread %d: c[%d]= %d\n",omp_get_thread_num(),i,c[i]);
                }
            }
#pragma omp section
            {
                for (int i=0; i<N; i++){
                    d[i] = a[i] * b[i];
                    printf("Thread %d: d[%d]= %d\n",omp_get_thread_num(),i,d[i]);
                }
            }
        }
    }
    return 0;
}
