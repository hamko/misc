#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

template<typename T>
void disp(const vector<T> &x) {
    for (size_t i = 0; i < x.size(); i++)
        cout << x[i] << " ";
    cout << endl;
}

int main(int argc, char **argv) {
    vector<int> a; for (int i = 0; i < 10; i++) a.push_back(i);

    // 二乗を計算（関数オブジェクトは一項演算）
    transform(a.begin(), a.end(), a.begin(), [](int x) {return x*x;});
    disp(a);   // 0 1 4 9 16 25 36 49 64 81

    // 二つの配列から一つの配列を作る（関数オブジェクトは二項演算）
    vector<int> d; for (int i = 0; i < 10; i++) d.push_back(i);
    vector<int> aa_a;
    transform(a.begin(), a.end(), d.begin(), back_inserter(aa_a), plus<int>());

    // 二つの配列として指定するものは自分の配列でもいい．フィボナッチ数列
    vector<int> fib(20);
    fib[0] = 1; fib[1] = 1;
    transform(fib.begin(), fib.end()-2, fib.begin()+1, fib.begin()+2, plus<int>());
    disp(fib);  // 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 1597 2584 4181 6765 

    // 入力と出力の型が違ってもOK
    vector<char> b;
    transform(a.begin(), a.end(), back_inserter(b), [](int x){return 'A'+x%26;});
    disp(b);   // A B E J Q Z K X M D 

    // デフォルト二項演算は+演算子(std::plus)
    // 使える提供された候補は，
    // minus, multiplies, divides, modulus, equal_to, not_equal_to, greater, less, greater_equal, less_equal
    // logical_and, logical_or, bit_and, bit_or, bit_xor
    // toupper, tolower
    cout << accumulate(a.begin(), a.end(), 0/*初期値*/) << endl;
    cout << accumulate(a.begin(), a.end(), 0/*初期値*/, plus<int>()) << endl;
    cout << accumulate(a.begin(), a.end(), 0/*初期値*/, [](int x, int y){return x + y;}) << endl;

    // for all, there exists
    cout << accumulate(a.begin(), a.end(), 1, [](int x, int y){return x && (y >= 0);}) << endl;
    cout << accumulate(a.begin(), a.end(), 1, [](int x, int y){return x && (y > 0);}) << endl;
    cout << accumulate(a.begin(), a.end(), 0, [](int x, int y){return x || (y <= 0);}) << endl;
    cout << accumulate(a.begin(), a.end(), 0, [](int x, int y){return x || (y < 0);}) << endl;
    
    // 全部同じ
    vector<int> same = {3, 3, 3, 3, 3};
    vector<bool> flags;
    transform(same.begin(), same.end() - 1, same.begin() + 1, back_inserter(flags), equal_to<int>());
    cout << accumulate(flags.begin(), flags.end(), 1, logical_and<bool>()) << endl;

    a.clear();
    for (int i = -5; i <= 5; i++)
      a.push_back(i);
    // 偶数のみ抽出
    vector<int> e;   
    copy_if(a.begin(), a.end(), back_inserter(e), [](int x) {return x % 2 == 0;});
    disp(e);      // -4 -2 0 2 4 

    // 負の数のみ抽出
    vector<int> c; 
    copy_if(a.begin(), a.end(), back_inserter(c), [](int x) {return x < 0;});
    disp(c);      // -5 -4 -3 -2 -1 


    return 0;
}
