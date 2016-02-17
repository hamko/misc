#include <iostream>
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_invoke.h"
#include "tbb/tick_count.h"

using namespace tbb;
using namespace std;
void f()
{
    cout << "f" << endl;
    sleep(1);
}

void bar(int a) 
{
    for (int i = 0; i < a; i++) 
        cout << "*";
    cout << endl;
    sleep(1);
}

int main()
{
    task_scheduler_init init;
    tick_count start, finish;

    start = tick_count::now(); 
    tbb::parallel_invoke(f, []{bar(80);}, []{bar(120);} );
    finish = tick_count::now();
    cout << "tbb=" << (finish - start).seconds() << endl;

    start = tick_count::now(); 
    f();bar(80);bar(120);
    finish = tick_count::now();
    cout << "serial=" << (finish - start).seconds() << endl;

    init.terminate();
    return 0;
}
