#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#define N 100000000

double gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main()
{
  int i;
  double mid, height, width;
  double sum=0.0;
  double area;
  double t1,t2;

  t1=gettimeofday_sec();
  
  width=1.0/(double) N;
  for(i=0;i<N;i++){
    mid=(i+0.5)*width;
    height=4.0/(1.0+mid*mid);
    sum+=height;
  }
  area=width*sum;

  t2=gettimeofday_sec();

  printf("pi=%1.20lf\n",area);
  printf("time=%lf [sec]\n",(double)(t2-t1));

  return 0;
}
