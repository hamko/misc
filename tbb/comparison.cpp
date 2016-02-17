#include <iostream>
#include <ctime>
#include <omp.h>
#include <cmath>
#include <cstdlib>
#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/tick_count.h"

using namespace std;
using namespace tbb;

const int task_num = 100;
const int rep = 10000000;

class Parallel
{
    private:
        int *v;

    public:
        void operator() (const blocked_range<int>& range) const 
        {
            for (int i = range.begin(); i < range.end(); i++) {
                long long sum = 0; for (int j = 0; j < rep; j++) { sum += j; }
                cout << sum << " ";
            }
        }

        Parallel(int buf[task_num]) : v(buf) {};
};


int main()
{
    int ccc[task_num];

    tick_count start, finish;

    // TBB
    start = tick_count::now();
    task_scheduler_init init;
    parallel_for( blocked_range<int>(0, task_num), Parallel(ccc), tbb::auto_partitioner());
    cout << endl;
    init.terminate();
    finish = tick_count::now();
    cout << "tbb=" << (finish - start).seconds() << endl;

    // Serial
    start = tick_count::now();
    for (int i = 0; i < task_num; i++) {
        long long sum = 0; for (int j = 0; j < rep; j++) { sum += j; }
        cout << sum << " ";
    }
    cout << endl;
    finish = tick_count::now();
    cout << "serial=" << (finish - start).seconds() << endl;

    // OpenMP
    start = tick_count::now();
    long long sum;
    int j;
#pragma omp parallel for private(sum, j)
    for (int i = 0; i < task_num; i++) {
        sum = 0; for (j = 0; j < rep; j++) { sum += j; }
        cout << sum << " ";
    }
    cout << endl;
    finish = tick_count::now();
    cout << "openmp=" << (finish - start).seconds() << endl;


    return 0;
}
