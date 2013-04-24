#include <iostream>
using namespace std;

#define NUM 6
#define SNUM 3

double v[NUM] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
double w[NUM] = {5.0, 4.0, 3.0, 2.0, 1.0, 0.5};

void swap(double *a, double *b)
{
    double c = *a;
    *a = *b;
    *b = c;
}

// include ==
int proposition(double x)
{
    double tmp[NUM];
    double sum = 0.0;
    for (int i = 0; i < NUM; i++)
        tmp[i] = v[i] - x * w[i];
    for (int i = 0; i < NUM - 1; i++) 
        for (int j = 0; j < NUM - i - 1; j++)
            if (tmp[j] < tmp[j+1])
                swap(&tmp[j], &tmp[j+1]);
    cout << "tmp:" << endl;
    for (int i = 0; i < NUM; i++) {
        cout << tmp[i] << endl;
    }
    for (int i = 0; i < SNUM; i++)
        sum += tmp[i];

    cout << "sum >= 0: " << (int)(sum >= 0) << endl;
    return sum >= 0;
}

int main(void)
{
    // [lb, ub)
    double lb = 0.0, ub = 10000.0, mid;
    for (int i = 0; i < 100; i++) {
        mid = (lb + ub) / 2;
        if (proposition(mid))
            lb = mid;
        else 
            ub = mid;
    }
    cout << lb << " <= " << mid << " < " << ub << endl;
    return 0;
}
