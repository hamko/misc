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
const int rep = 100000000;

class Parallel
{
    private:
        long long *v;

    public:
        void operator() (const blocked_range<int>& range) const 
        {
            for (int i = range.begin(); i < range.end(); i++) {
                long long sum = 0; for (int j = 0; j < rep; j++) { sum += j; }
                cout << sum << " ";
                v[i] = sum;
            }
        }

        Parallel(long long buf[task_num]) : v(buf) {};
};


int main()
{
    task_scheduler_init init;
    long long ccc[task_num];

    tick_count start, finish;

    // TBB
    start = tick_count::now();
    parallel_for( blocked_range<int>(0, task_num), Parallel(ccc));
    cout << endl;
    finish = tick_count::now();
    cout << "tbb=" << (finish - start).seconds() << endl;

    // TBB with Lambda
    start = tick_count::now();
    parallel_for( blocked_range<int>(0, task_num), 
            [&](const blocked_range<int>& range) {
                for (int i = range.begin(); i < range.end(); i++) {
                    long long sum = 0; for (int j = 0; j < rep; j++) { sum += j; }
                    cout << sum << " ";
                    ccc[i] = sum;
                }
            });
    cout << endl;
    finish = tick_count::now();
    cout << "tbb with lambda=" << (finish - start).seconds() << endl;

    // TBB with Lambda Simple
    start = tick_count::now();
    parallel_for(0, task_num, 
            [&](size_t i) {
                long long sum = 0; for (int j = 0; j < rep; j++) { sum += j; }
                ccc[i] = sum;
                cout << sum << " ";
            }
            , tbb::auto_partitioner());
    cout << endl;
    finish = tick_count::now();
    cout << "tbb with lambda simple=" << (finish - start).seconds() << endl;


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
        ccc[i] = sum;
    }
    cout << endl;
    finish = tick_count::now();
    cout << "openmp=" << (finish - start).seconds() << endl;


    init.terminate();
    return 0;
}
