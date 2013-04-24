#include <iostream>

using namespace std;

int exgcd(int a, int b, int* x, int* y)
{
    if (b) {
        int prex, prey;
        int tmp = exgcd(b, a % b, &prex, &prey);
        *x = prey;
        *y = prex - a / b * prey;
        return tmp;
    } else {
        *x = 1;
        *y = 0;
        return a;
    }
}

int main(void)
{
    int a = 129409;
    int b = 38750;
    int x, y;
    int g = exgcd(a, b, &x, &y);
    cout << "a   : " << a << endl;
    cout << "b   : " << b << endl;
    cout << "gcd : " << g << endl;
    cout << "a * x + b * y = gcd" << endl;
    cout << a << " * " << x << " + " << b << " * " << y << " = " << g << endl;

    return 0;
}
