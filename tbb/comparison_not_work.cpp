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

const int x = 100;
const int rep = 1000000;

class Parallel
{
    private:
        float *v;

    public:
        void operator() (const blocked_range<int>& range) const 
        {
            // 0～7までの値を入れる
            cout << range.begin() << " " << range.end() << endl;
            for (int i = range.begin(); i < range.end(); i++) {
                int sum = 0; for (int j = 0; j < rep; j++) { sum += rand(); }
                v[i] = sin(sum % 8);
            }
        }

        Parallel(float buf[x]) : v(buf) {};
};


int main()
{
    srand((unsigned)time(NULL));
    float ccc[x];

    tick_count start, finish;

    // TBB
    start = tick_count::now();
    task_scheduler_init init;
    parallel_for( blocked_range<int>(0, x), Parallel(ccc), tbb::auto_partitioner());
    init.terminate();
    finish = tick_count::now();
    cout << "tbb=" << (finish - start).seconds() << endl;

    // Serial
    start = tick_count::now();
    for (int i = 0; i < x; i++) {
        int sum = 0; for (int j = 0; j < rep; j++) { sum += rand(); }
        ccc[i] = sin(sum % 8);
    }
    finish = tick_count::now();
    cout << "serial=" << (finish - start).seconds() << endl;

    // OpenMP
    start = tick_count::now();
    int sum, j;
#pragma omp parallel for private(sum, j)
    for (int i = 0; i < x; i++) {
        sum = 0; for (j = 0; j < rep; j++) { sum += rand(); }
        ccc[i] = sin(sum % 8);
        cout << omp_get_thread_num() << endl;
    }
    finish = tick_count::now();
    cout << "openmp=" << (finish - start).seconds() << endl;


    return 0;
}
