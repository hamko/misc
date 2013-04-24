#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

extern int gl_main(int argc, char** argv);

void *myThread(void *arg)
{
    gl_main(0, NULL);
    return NULL;
}

extern int radian;
int main(int argc,char *argv[])
{
    int status;

    // スレッドa用のパラメータ
    pthread_t thread_a;
    if(!pthread_create(&thread_a, NULL, myThread, NULL)) {
        fprintf(stderr,"failed to create thread_a\n");
        exit(1);
    }

    while (1) {
        radian++;
        sleep(1);
    }

    return 0;
}
