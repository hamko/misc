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
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(a) ((a)>0?(a):-(a))

class KingXNewBaby {
public:
  string isValid(string name);

  
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); if ((Case == -1) || (Case == 5)) test_case_5(); if ((Case == -1) || (Case == 6)) test_case_6(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const string &Expected, const string &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arg0 = "dengklek"; string Arg1 = "YES"; verify_case(0, Arg1, isValid(Arg0)); }
	void test_case_1() { string Arg0 = "gofushar"; string Arg1 = "NO"; verify_case(1, Arg1, isValid(Arg0)); }
	void test_case_2() { string Arg0 = "dolphinigle"; string Arg1 = "NO"; verify_case(2, Arg1, isValid(Arg0)); }
	void test_case_3() { string Arg0 = "mystictc"; string Arg1 = "NO"; verify_case(3, Arg1, isValid(Arg0)); }
	void test_case_4() { string Arg0 = "rngringo"; string Arg1 = "NO"; verify_case(4, Arg1, isValid(Arg0)); }
	void test_case_5() { string Arg0 = "misof"; string Arg1 = "NO"; verify_case(5, Arg1, isValid(Arg0)); }
	void test_case_6() { string Arg0 = "metelsky"; string Arg1 = "YES"; verify_case(6, Arg1, isValid(Arg0)); }

// END CUT HERE

};

string KingXNewBaby::isValid(string name) {
    char vo[5] = {}; 
    
    if (name.size() != 8)
        return "NO";

    for (int i = 0; i < 8; i++) 
        if (name[i] == 'a')
            vo[0]++;
        else if (name[i] == 'i')
            vo[1]++;
        else if (name[i] == 'u')
            vo[2]++;
        else if (name[i] == 'e')
            vo[3]++;
        else if (name[i] == 'o')
            vo[4]++;

    int sum = 0;
    for (int i = 0; i < 5; i++) 
        sum += vo[i];
    if (sum != 2)
        return "NO";

    int i;
    for (i = 0; i < 5; i++)
        if (vo[i] == 2)
            break;
    return i == 5 ? "NO" : "YES";
}

// BEGIN CUT HERE
int main() {
  KingXNewBaby ___test;
  ___test.run_test(-1);
}
// END CUT HERE
