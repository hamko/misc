#include <algorithm>
#include <iostream>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_sort.h"
#include "tbb/tick_count.h"
#include <math.h>

using namespace tbb;
using namespace std;

const int N = 1000000;
float a[N];

int main(void)
{
    task_scheduler_init init;
    tick_count start, finish;

    // O(n log n)の並列不安定ソート
    for( int i = 0; i < N; i++ ) a[i] = sin((double)i);
    start = tick_count::now(); 
    parallel_sort(a, a + N);
    finish = tick_count::now();
    cout << "tbb=" << (finish - start).seconds() << endl;

    // O(n log n)の不安定ソート
    for( int i = 0; i < N; i++ ) a[i] = sin((double)i);
    start = tick_count::now(); 
    std::sort(a, a + N);
    finish = tick_count::now();
    cout << "serial=" << (finish - start).seconds() << endl;

    init.terminate();

    return 0;
}
