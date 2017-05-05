#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

void *task1(void *arg)
{
  int i;
  pthread_t thread;

  thread=pthread_self();
  for(i=0;i<10;i++){
    sleep(1);
    printf("Theread [%d]: %d\n",(int)thread,i);
  }
  return(NULL);
}
void *task2(void *arg)
{
  int i;
  pthread_t thread;

  thread=pthread_self();
  for(i=0;i<10;i++){
    sleep(2);
    printf("Theread [%d]: %d\n",(int)thread,i);
  }
  return(NULL);
}


int main()
{
  pthread_t thread1, thread2, thread3;

  pthread_create(&thread1,NULL,task1,(void *)NULL);
  pthread_create(&thread2,NULL,task1,(void *)NULL);
  pthread_create(&thread3,NULL,task2,(void *)NULL);

  pthread_join(thread1,NULL);
  printf("Thread %d finished.\n", (int)thread1);
  pthread_join(thread2,NULL);
  printf("Thread %d finished.\n", (int)thread2);

  pthread_join(thread3,NULL);
  printf("Thread %d finished.\n", (int)thread3);
  return 0;
}
