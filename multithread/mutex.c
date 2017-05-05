#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int flag=1;
int balance=10000;
int cash=0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#define L pthread_mutex_lock(&lock)
#define U pthread_mutex_unlock(&lock)
void *task1(void *arg)
{
  int t;
  int balanceread;

  L;
  while (flag) {
    t=rand()%1000+1000;
    if (cash >= 5) {
      balanceread = balance;
      balanceread += 5;
      cash -= 5;
      usleep(t);
      balance = balanceread;
    }
    /*printf("task1: balance=%d cash=%d total=%d\n", balance, cash, balance+cash);*/
  }
  U;
  return(NULL);
}

void *task2(void *arg)
{
  int t;
  int balanceread;

  L;
  while (flag) {
    t=rand()%1000;
    balanceread = balance;
    if (balanceread >= 3) {
      balanceread -= 3;
      cash += 3;
      usleep(t);
      balance = balanceread;
    }
    /*printf("task2: balance=%d cash=%d total=%d\n", balance, cash, balance+cash);*/
  }
  U;
  return(NULL);
}

int main()
{
  pthread_t thread1, thread2;

  printf("balance=%d cash=%d total=%d\n", balance, cash, balance+cash);
  srand(time(NULL));

  pthread_create(&thread1,NULL,task1,(void *)NULL);
  pthread_create(&thread2,NULL,task2,(void *)NULL);

  sleep(3);
  flag=0;

  pthread_join(thread1,NULL);
  printf("Thread %u finished.\n", (int)thread1);
  pthread_join(thread2,NULL);
  printf("Thread %u finished.\n", (int)thread2);

  printf("balance=%d cash=%d total=%d\n", balance, cash, balance+cash);

  return 0;
}
