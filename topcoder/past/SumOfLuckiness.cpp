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

class SumOfLuckiness {
public:
  long long theSum(int A, int B);

  
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const long long &Expected, const long long &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arg0 = 1; int Arg1 = 10; long long Arg2 = 2LL; verify_case(0, Arg2, theSum(Arg0, Arg1)); }
	void test_case_1() { int Arg0 = 40; int Arg1 = 47; long long Arg2 = 8LL; verify_case(1, Arg2, theSum(Arg0, Arg1)); }
	void test_case_2() { int Arg0 = 58; int Arg1 = 526; long long Arg2 = 231LL; verify_case(2, Arg2, theSum(Arg0, Arg1)); }
	void test_case_3() { int Arg0 = 4444; int Arg1 = 7777; long long Arg2 = 2338LL; verify_case(3, Arg2, theSum(Arg0, Arg1)); }
	void test_case_4() { int Arg0 = 585858585; int Arg1 = 858585858; long long Arg2 = 287481025LL; verify_case(4, Arg2, theSum(Arg0, Arg1)); }

// END CUT HERE

};

int get_d_num(int i, int d) {
    if (i == 0)
        return 0;
    return get_d_num(i / 10, d) + ((i-(i/10)*10) == d);
}
#define ABS(a) ((a)>0?(a):-(a))
long long SumOfLuckiness::theSum(int A, int B) {
    long long result = 0;
    for (int i = A; i < B; i++) {
        int four = get_d_num(i, 4);
        int seven = get_d_num(i, 7);
        result += ABS(four - seven);
    } 

    return result;
}

// BEGIN CUT HERE
int main() {
  SumOfLuckiness ___test;
  ___test.run_test(-1);
}
// END CUT HERE
