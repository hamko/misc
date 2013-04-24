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

class LuckyRemainder {
public:
  int getLuckyRemainder(string X);

  
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arg0 = "123"; int Arg1 = 6; verify_case(0, Arg1, getLuckyRemainder(Arg0)); }
	void test_case_1() { string Arg0 = "24816"; int Arg1 = 3; verify_case(1, Arg1, getLuckyRemainder(Arg0)); }
	void test_case_2() { string Arg0 = "8"; int Arg1 = 8; verify_case(2, Arg1, getLuckyRemainder(Arg0)); }
	void test_case_3() { string Arg0 = "11235813213455"; int Arg1 = 7; verify_case(3, Arg1, getLuckyRemainder(Arg0)); }

// END CUT HERE

};

int conb(int n, int m)
{
    if (m <= 0)
        return 1;
    if (n < m)
        return 0;
    return conb(n-1,m) + conb(n-1,m-1);
}

int LuckyRemainder::getLuckyRemainder(string X) {
    int a[50] = {};
    for (int i = 0; i < 50; i++) 
        a[i] = -1;
    int len = X.length();
    for (int i = 0; i < len; i++) 
        a[i] = X[i] - '0';
    int strsum = 0;
    for (int i = 0; i < len; i++)
        strsum += a[i];

    int result = 0;
    for (int i = 0; i < len; i++) {
        result += conb(len - 1, i - 1) * strsum;
        result %= 9;
    }
        
    return result;
}

// BEGIN CUT HERE
int main() {
  LuckyRemainder ___test;
  ___test.run_test(-1);
}
// END CUT HERE
