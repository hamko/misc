#include <iostream>
#include "stopwatch_util.hpp"
#include <omp.h>

using namespace std;
#define N 10000000

// NG
// thread0がsumを読む→thread1がsumを読む→thread0がiを足す→thread1がiを足す、って感じで死
/* 一つのforスコープ指示文の中で、共有変数は読むか書くかのどちらかしかしてはならない */
/* 本当にやりたいなら、読み込み＋更新＋書き込みを同時に行うことを保障する、atomic指定子を付ける(むちゃくちゃ遅い) */
void calc_sum_NG(void)
{
    long long sum = 0; 
    stopwatch_start(0);
#pragma omp parallel
    {
#pragma omp for 
        for (int i = 0; i < N; i++) 
            sum += i;
    }
    double time = stopwatch_end(0);

    cout << time << " " << sum << "#NG result" << endl;
}

// OK
void calc_sum(void)
{
    long long sum = 0; 
    stopwatch_start(0);
#pragma omp parallel 
    {
#pragma omp for reduction(+:sum)
        for (int i = 0; i < N; i++) 
            sum += i;
#pragma omp single 
        {
            cout << "single" << endl;
        }
        cout << "parallel" << endl;
    }
    double time = stopwatch_end(0);

    cout << time << " " << sum << "#OK result" << endl;
}

int main (void )
{
    // NG
    cout << "NG" << endl;
    calc_sum_NG();
    // OK
    cout << "OK" << endl;
    calc_sum();
    return 0;
}
