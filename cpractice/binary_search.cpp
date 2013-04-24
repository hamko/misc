#include <iostream>
#include <cstdio>
#define NUM 6
#define TOFIND 3

using namespace std;

int main(void)
{
    int a[NUM] = {0, 1, 3, 4, 5, 6};
    int lb, ub;

    /* [lb, ub) */
    // 探索空間を配列全体にする初期化をする
    lb = 0; ub = NUM;
    while (ub - lb > 1) {
        int mid = (ub + lb) / 2;
        if (a[mid] <= TOFIND)
            lb = mid;
        else
            ub = mid;
    }
    // 下が開なので、ubは探索している値になってはならない。
    // よって、複数同じ値がある場合は最も添字が大きい値になる。
    cout << "find " << TOFIND << endl;
    cout << "lb: " << a[lb] << endl;
    cout << "ub: " << a[ub] << endl;

    return 0;
}
