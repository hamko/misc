#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <cfloat>
#include <map>
#include <utility>
#include <set>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>
#include <complex>
#include <stack>
#include <queue>
using namespace std;
static const double EPS = 1e-5;
typedef long long ll;

class MergersDivTwo {
public:
  double findMaximum(vector <int> revenues, int k);

  
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const double &Expected, const double &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arr0[] = {5, -7, 3}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 2; double Arg2 = 1.5; verify_case(0, Arg2, findMaximum(Arg0, Arg1)); }
	void test_case_1() { int Arr0[] = {5, -7, 3}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 3; double Arg2 = 0.3333333333333333; verify_case(1, Arg2, findMaximum(Arg0, Arg1)); }
	void test_case_2() { int Arr0[] = {1, 2, 2, 3, -10, 7}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 3; double Arg2 = 2.9166666666666665; verify_case(2, Arg2, findMaximum(Arg0, Arg1)); }
	void test_case_3() { int Arr0[] = {-100, -100, -100, -100, -100, 100}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 4; double Arg2 = -66.66666666666667; verify_case(3, Arg2, findMaximum(Arg0, Arg1)); }
	void test_case_4() { int Arr0[] = {869, 857, -938, -290, 79, -901, 32, -907, 256, -167, 510, -965, -826, 808, 890,
 -233, -881, 255, -709, 506, 334, -184, 726, -406, 204, -912, 325, -445, 440, -368}; vector <int> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 7; double Arg2 = 706.0369290573373; verify_case(4, Arg2, findMaximum(Arg0, Arg1)); }

// END CUT HERE

};

void swap(int* a, int* b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

double div_k_cyc(int rev, int cyc, int k)
{
    double ret = (double)rev;
    long long p = 1;
    for (int i = 0; i < cyc; i++) 
        p *= k;

    return ret / p;
}

double MergersDivTwo::findMaximum(vector <int> revenues, int k) {
    int num = revenues.size();
    for (int i = 0; i < num - 1; i++) 
        for (int j = 0; j < num - i - 1; j++) 
            if (revenues[j] > revenues[j+1])
                swap(&revenues[j], &revenues[j+1]);

    int size = num;
    double result = 0.0;
    int cyc = 0;
    while (1) {
        if (size < 2 * k - 1) {
            int sum = 0;
            for (int i = 0; i < size; i++) 
                sum += revenues[i];

            result += (double)div_k_cyc(sum, cyc, k) / size;
            break;
        }
        cyc++;

        for (int i = 0; i < k - 1; i++) 
            result += div_k_cyc(revenues[size-i-1], cyc, k);
        size -= (k - 1);
    }

    return result;
}

// BEGIN CUT HERE
int main() {
  MergersDivTwo ___test;
  ___test.run_test(-1);
}
// END CUT HERE
