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

class DengklekMakingChains {
public:
  int maxBeauty(vector <string> chains);

  
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {".15", "7..", "402", "..3"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 19; verify_case(0, Arg1, maxBeauty(Arg0)); }
	void test_case_1() { string Arr0[] = {"..1", "7..", "567", "24.", "8..", "234"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 36; verify_case(1, Arg1, maxBeauty(Arg0)); }
	void test_case_2() { string Arr0[] = {"...", "..."}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 0; verify_case(2, Arg1, maxBeauty(Arg0)); }
	void test_case_3() { string Arr0[] = {"16.", "9.8", ".24", "52.", "3.1", "532", "4.4", "111"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 28; verify_case(3, Arg1, maxBeauty(Arg0)); }
	void test_case_4() { string Arr0[] = {"..1", "3..", "2..", ".7."}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 7; verify_case(4, Arg1, maxBeauty(Arg0)); }
	void test_case_5() { string Arr0[] = {"412", "..7", ".58", "7.8", "32.", "6..", "351", "3.9", "985", "...", ".46"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 58; verify_case(5, Arg1, maxBeauty(Arg0)); }

// END CUT HERE

};

#define MAX(a,b) ((a)>(b)?(a):(b))
int DengklekMakingChains::maxBeauty(vector <string> chains) {
    int right = 0, left = 0, mid = 0;
    int sol = 0;
    int br[100];
    int bl[100];
    int n = 0;
    
    for (int i = 0; i < chains.size(); i++) {
        if (chains[i][0] == '.' && chains[i][1] == '.' && chains[i][2] != '.') 
            right = MAX(right, chains[i][2] - '0');
        else if (chains[i][0] == '.' && chains[i][1] != '.' && chains[i][2] != '.') 
            right = MAX(right, chains[i][2] + chains[i][1] - 2 * '0');
        else if (chains[i][0] != '.' && chains[i][1] == '.' && chains[i][2] == '.') 
            left = MAX(left, chains[i][0] - '0');
        else if (chains[i][0] != '.' && chains[i][1] != '.' && chains[i][2] == '.') 
            left = MAX(left, chains[i][0] + chains[i][1] - 2 * '0');
        else if (chains[i][0] == '.' && chains[i][1] != '.' && chains[i][2] == '.') 
            sol = MAX(sol, chains[i][1] - '0');
        else if (chains[i][0] != '.' && chains[i][1] != '.' && chains[i][2] != '.') 
            mid += chains[i][0] + chains[i][1] + chains[i][2] - 3 * '0';
        else if (chains[i][0] != '.' && chains[i][1] == '.' && chains[i][2] != '.') {
            br[n] = chains[i][0] - '0';
            bl[n] = chains[i][2] - '0';
            n++;
        }
    }
    
    br[n] = 0; bl[n] = right;
    n++;
    br[n] = left; bl[n] = 0;
    n++;

    int ijmax = 0;
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++)
            if (i != j && ijmax < br[i] + bl[j]) 
                ijmax = br[i] + bl[j];

    return MAX(ijmax + mid, sol);
}

// BEGIN CUT HERE
int main() {
  DengklekMakingChains ___test;
  ___test.run_test(-1);
}
// END CUT HERE
