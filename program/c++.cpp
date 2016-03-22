#include <bits/stdc++.h>
#define N 3

using namespace std;

int func_pow2(int a) 
{
    return a * a;
}

int main(void)
{
    cout << 2 + 3 << endl;
    cout << 2 - 3 << endl;
    cout << 2 * 3 << endl;
    cout << 2 / 3 << endl;
    cout << 2 % 3 << endl;
    cout << (2 == 2) << endl;
    cout << (2 == 1) << endl;
    cout << (2 != 2) << endl;
    cout << (2 != 1) << endl;
    cout << (2 > 2) << endl;
    cout << (2 > 1) << endl;
    cout << (2 >= 2) << endl;
    cout << (2 >= 1) << endl;
    cout << (1 && 1) << endl;
    cout << (1 || 1) << endl;
    cout << (1 & 2) << endl;
    cout << (1 | 1) << endl;

    int a[3];
    /*
    for (int i = 0; i < 3; i++) {
        cout << a[i] << endl; // NG 未初期化変数の使用
    }
    */
    for (int i = 0; i < 3; i++) {
        a[i] = i;
    }
    // a[-1] = -1; // NG 領域外参照
    // a[3] = 3; // NG 領域外参照
    for (int i = 0; i < 3; i++) 
        cout << a[i] << " ";
    cout << endl;
    for (int i = 0; i < 3; i++) 
        cout << *(a + i) << " ";
    cout << endl;

    int a_init[3] = {};
    for (int i = 0; i < 3; i++) {
        cout << a_init[i] << endl; // = {};とすると、全変数が0で初期化される。
    }

    int b[3][4];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            b[i][j] = i * j;
        }
    }

    if (1) {
        cout << "Displayed" << endl;
    } else {
        cout << "Never Displayed" << endl;
    }
    if (0) 
        cout << "Never Displayed" << endl;
    else 
        cout << "Displayed" << endl;
    if (2 == 0) {
        cout << "Never Displayed" << endl;
    } else if (2 == 1) {
        cout << "Never Displayed" << endl;
    } else {
        cout << "Displayed" << endl;
    }

    cout << func_pow2(4) << endl;

    vector<int> v0;
    v0.push_back(1);
    v0.push_back(2);
    cout << v0.size() << endl;
    for (auto it = v0.begin(); it != v0.end(); it++) {
        cout << *it << " "; 
    }
    cout << endl;

    vector<int> v1(3);
    cout << v1.size() << endl;

    vector<int> v2(3, 1);
    cout << v2.size() << endl;
    for (int i = 0; i < v2.size(); i++) {
        cout << v2[i] << " "; 
    }
    cout << endl;

    /*
    for (int i = 0; i < v2.size() - 5; i++) { // NG v2.size()はunsigned intなので、範囲指定の時は注意
        cout << v2[i] << " "; 
    }
    cout << endl;
    */
    for (int i = 0; i < (int)v2.size() - 5; i++) { // OK
        cout << v2[i] << " "; 
    }
    cout << endl;

    vector<int> v3 = {3, 2, 1};
    sort(v3.begin(), v3.end());
    for (int i = 0; i < v3.size(); i++) {
        cout << v3[i] << " "; 
    }
    cout << endl;
    sort(v3.begin(), v3.end(), greater<int>());
    for (int i = 0; i < v3.size(); i++) {
        cout << v3[i] << " "; 
    }
    cout << endl;


    return 0;
}
