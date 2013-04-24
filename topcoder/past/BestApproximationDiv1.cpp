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

class BestApproximationDiv1 {
public:
  string findFraction(int maxDen, string number);

  
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { int Arg0 = 42; string Arg1 = "0.141592"; string Arg2 = "1/7 has 3 exact digits"; verify_case(0, Arg2, findFraction(Arg0, Arg1)); }
	void test_case_1() { int Arg0 = 3; string Arg1 = "0.133700"; string Arg2 = "0/1 has 1 exact digits"; verify_case(1, Arg2, findFraction(Arg0, Arg1)); }
	void test_case_2() { int Arg0 = 1000; string Arg1 = "0.123456"; string Arg2 = "10/81 has 7 exact digits"; verify_case(2, Arg2, findFraction(Arg0, Arg1)); }
	void test_case_3() { int Arg0 = 1000; string Arg1 = "0.420000"; string Arg2 = "21/50 has 7 exact digits"; verify_case(3, Arg2, findFraction(Arg0, Arg1)); }
	void test_case_4() { int Arg0 = 100; string Arg1 = "0.909999"; string Arg2 = "10/11 has 4 exact digits"; verify_case(4, Arg2, findFraction(Arg0, Arg1)); }
	void test_case_5() { int Arg0 = 115; string Arg1 = "0.141592"; string Arg2 = "16/113 has 7 exact digits"; verify_case(5, Arg2, findFraction(Arg0, Arg1)); }

// END CUT HERE

};

int correspond(int m, int n, int d)
{
    for (int i = 0; i < d - 1; i++) {
        m /= 10;
        n /= 10;
    }
    int dm = m - (m / 10) * 10;
    int dn = n - (n / 10) * 10;
    return dm == dn;
}

string BestApproximationDiv1::findFraction(int maxDen, string number) {
    int num = 0;
    int i = 2;
    while (1) {
        num += number[i]-'0';
        if (i >= number.length() - 1)
            break;
        num *= 10;
        i++;
    }
        
    int maxi = 1, maxj = 0;
    int maxijperf = 0;
    for (int i = 1; i <= maxDen; i++) {
        for (int j = 0; j < i; j++) {
            int tmp = 1000000 * j / i;
            int ijperf = 0;
            for (int h = 0; h < 6; h++) {
                if (correspond(tmp, num, 6 - h))
                    ijperf++;
                else
                    break;
            }
            if (maxijperf < ijperf) {
                maxijperf = ijperf;
                maxi = i;
                maxj = j;
            } else if (maxijperf == ijperf) {
                if (maxi > i) {
                    maxi = i;
                    maxj = j;
                } 
            }
        }
    }
    
    string result;
    char str[100];
    sprintf(str, "%d", maxj);
    result += str;
    result += "/";
    sprintf(str, "%d", maxi);
    result += str;
    result += " has ";
    sprintf(str, "%d", maxijperf + 1);
    result += str;
    result += " exact digits";
    return result;
}

// BEGIN CUT HERE
int main() {
  BestApproximationDiv1 ___test;
  ___test.run_test(-1);
}
// END CUT HERE
