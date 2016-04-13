// 演算子の優先順位は、キャスト、*、+の順番。
// 異なる型同士の二項演算子は、精度の低い方を暗黙に精度が高い方にキャストする。

#include <bits/stdc++.h> 
using namespace std; 
int main(void) 
{ 
    int n; 
    cin >> n;

    long long a;
    a/*NG*/ = n * n * n; cout << a << endl;
    a/*OK*/ = (long long)n * n * n; cout << a << endl;
    a/*NG*/ = (long long)n * n * n - n * n * n; cout << a << endl;
    a/*OK*/ = (long long)n * n * n - (long long)n * n * n; cout << a << endl;
    return 0;
}
