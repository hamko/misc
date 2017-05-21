#include <stdio.h>

int main(void) {
#pragma omp parallel
    {
        printf("hello\n");
    }

    return 0;
}
