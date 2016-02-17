#include <iostream>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_reduce.h"
#include "tbb/blocked_range.h"
#include "tbb/tick_count.h"

using namespace tbb;
using namespace std;
#define NUM 40000000000
//#define WITH_ARRARY

int main()
{
    task_scheduler_init init;
    tick_count start, finish;

#ifdef WITH_ARRARY
    int array[NUM];
    parallel_for(0, NUM, 
            [&] (size_t i) {
                array[i] = i;
            });

    // tbb
    start = tick_count::now();
    cout <<  
        parallel_reduce(blocked_range<int*>( array, array+NUM ), (long long)0, 
            // リーフ
            [](const blocked_range<int*>& r, long long init)->long long {
                for( int* a=r.begin(); a!=r.end(); ++a ) 
                init += *a;
                return init;
            },
            // ボティの連結
            []( long long x, long long y )->long long {
                return x+y;
            })
    << endl;                  
    finish = tick_count::now();
    cout << "tbb=" << (finish - start).seconds() << endl;

    // serial
    start = tick_count::now();
    long long sum = 0;
    for (size_t i = 0; i < NUM; i++) {
        sum += array[i];
    }
    cout << sum << endl;
    finish = tick_count::now();
    cout << "serial=" << (finish - start).seconds() << endl;
#endif

    // tbb without array
    start = tick_count::now();
    cout <<  
        parallel_reduce(blocked_range<long long>( 0, NUM ), (long long)0, 
            // リーフ
            [](const blocked_range<long long>& r, long long init)->long long {
                cout << r.begin() << " " << r.end() << endl;
                for( long long a=r.begin(); a!=r.end(); ++a ) 
                init += a;
                return init;
            },
            // ボティの連結
            []( long long x, long long y )->long long {
                return x+y;
            })
    << endl;                  
    finish = tick_count::now();
    cout << "tbb without array=" << (finish - start).seconds() << endl;

    // serial without array
    start = tick_count::now(); 
    long long sum = 0;
    for (long long i = 0; i < NUM; i++) {
        sum += i;
    }
    cout << sum << endl;
    finish = tick_count::now();
    cout << "serial without array=" << (finish - start).seconds() << endl;

    init.terminate();
    return 0;
}
