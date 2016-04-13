#include <bits/stdc++.h> 
using namespace std; 
int main(void) 
{ 
    int n; 
    cin >> n;

    long long a;
    a = n * n * n; cout << a << endl;
    a = (long long)n * n * n; cout << a << endl;
    a = (long long)n * n * n - n * n * n; cout << a << endl;
    a = (long long)n * n * n - (long long)n * n * n; cout << a << endl;
    return 0;
}
