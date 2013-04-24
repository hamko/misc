#include <cstdio>
#include <iostream>
#define TOFIND 1.23456

using namespace std;

int main(void)
{
    double lb, ub;

    // [lb, ub)
    lb = 0.0; ub = 1.0e+4;
    while (ub - lb > 1e-3) {
        double mid = (lb + ub) / 2;
        if (mid <= TOFIND)
            lb = mid;
        else
            ub = mid;
    }
    cout << lb << " < " << TOFIND << " <= " << ub << endl;

    return 0;
}
