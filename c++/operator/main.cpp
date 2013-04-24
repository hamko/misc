#include <iostream>
#include "operator.hpp"

using namespace std;

int main(int argc, char** argv)
{
    Test a = Test(3);
    Test b = Test(3);
    Test c = Test(5);
    Test r = a + b + c;

    // +
    cout << a.getNum() << " " << b.getNum() << " " << c.getNum() << " " << r.getNum() << endl;

    // =
    a = c;
    cout << a.getNum() << endl;

    // =
    a = a[c];
    cout << a.getNum() << endl;
    a = a[3];
    cout << a.getNum() << endl;

    return 0;
}
