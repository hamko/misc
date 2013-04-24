#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "params.h"

float io_mdats[MAX_DATA][MAX_PHOTOSENS];
int cyc;
void input(void)
{
    FILE *fp;
    if (!(fp = fopen("input", "r"))) {
        perror("file");
        exit(1);
    }

    char tmp[1024];
    int i = 0;
    while (fgets(tmp, sizeof(tmp), fp)) {
        int j;
        for (j = 0; j < MDAT_NUM; j++)
            io_mdats[i][j] = tmp[j] - '0';
        i++;
    }
    cyc = i;

    fclose(fp);
}

void output(void)
{
}
