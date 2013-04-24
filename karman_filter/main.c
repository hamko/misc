#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

double BoxMuller(double sigma, double mean)
{
    double alpha, beta;
    if ((alpha = (double)rand() / RAND_MAX) == 0)
        return 0.0;
    beta = (double)rand() / RAND_MAX;

    double bm = sigma * sqrt(-2 * log(alpha)) * cos(2 * M_PI * beta) + mean;
    printf("#RAND %f %f %f\n", alpha, beta, bm);
    return sigma * sqrt(-2 * log(alpha)) * cos(2 * M_PI * beta) + mean;
}

#define REAL_VARIANCE 0.05
int main(void)
{
    FILE *fp;
    if ((fp = fopen("output.csv", "w")) == NULL) {
        perror("file:");
        exit(1);
    }
    fprintf(fp, "# iteration measure true X^ K\n");

    int num;
    double volt, x[2], p[2], K, R, Q, z;
    volt = 0.545;
    srand(time(NULL));

    x[0] = 0;
    p[0] = 0;

    Q = 1.0;
    R = 0.0025; 
    z = 0;

    for (num = 0; num < 2000; num++) {
        // time update
        x[1] = x[0];
        p[1] = p[0] + Q;
        
        // measurement update
        K = p[1]/(p[1]+R);

        z = volt + BoxMuller(REAL_VARIANCE, 0);
        x[0] = x[1] + K * (z - x[1]);
        p[0] = (1 - K) * p[1];

        fprintf(fp, "%d %f %f %f %f\n", num, z, volt, x[0], p[0], K);
    }

    printf("done.\n");
}

