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

class DucksAlignment {
public:
  int minimumTime(vector <string> grid);

  
// BEGIN CUT HERE
	public:
	void run_test(int Case) { if ((Case == -1) || (Case == 0)) test_case_0(); if ((Case == -1) || (Case == 1)) test_case_1(); if ((Case == -1) || (Case == 2)) test_case_2(); if ((Case == -1) || (Case == 3)) test_case_3(); if ((Case == -1) || (Case == 4)) test_case_4(); }
	private:
	template <typename T> string print_array(const vector<T> &V) { ostringstream os; os << "{ "; for (typename vector<T>::const_iterator iter = V.begin(); iter != V.end(); ++iter) os << '\"' << *iter << "\","; os << " }"; return os.str(); }
	void verify_case(int Case, const int &Expected, const int &Received) { cerr << "Test Case #" << Case << "..."; if (Expected == Received) cerr << "PASSED" << endl; else { cerr << "FAILED" << endl; cerr << "\tExpected: \"" << Expected << '\"' << endl; cerr << "\tReceived: \"" << Received << '\"' << endl; } }
	void test_case_0() { string Arr0[] = {".o",
 "o."}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 1; verify_case(0, Arg1, minimumTime(Arg0)); }
	void test_case_1() { string Arr0[] = {".o...",
 "..o..",
 "....o"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 3; verify_case(1, Arg1, minimumTime(Arg0)); }
	void test_case_2() { string Arr0[] = {"o..........",
 "..o........",
 ".......o...",
 "...........",
 "...........",
 "...........",
 "........o..",
 "..........."}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 16; verify_case(2, Arg1, minimumTime(Arg0)); }
	void test_case_3() { string Arr0[] = {".........",
 "....o....",
 "........."}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 0; verify_case(3, Arg1, minimumTime(Arg0)); }
	void test_case_4() { string Arr0[] = {"...o..........................",
 "............................o.",
 ".o............................",
 "............o.................",
 ".................o............",
 "......................o.......",
 "......o.......................",
 "....o.........................",
 "...............o..............",
 ".......................o......",
 "...........................o..",
 ".......o......................"}; vector <string> Arg0(Arr0, Arr0 + (sizeof(Arr0) / sizeof(Arr0[0]))); int Arg1 = 99; verify_case(4, Arg1, minimumTime(Arg0)); }

// END CUT HERE

};

typedef struct _cor {
    int x;
    int y;
} cor;
#define ABS(a) ((a)>0?(a):-(a))
int DucksAlignment::minimumTime(vector <string> grid) {
    vector <cor> ducks;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].length(); j++) {
            if (grid[i][j] == 'o') {
                cor tmp = {i, j};
                ducks.push_back(tmp);
            }
        }
    }
    int result = 10000000;
    for (int i = 0; i < grid.size(); i++) {
        int d = 0;
        for (int j = 0; j < ducks.size(); j++) {
            d += ABS(i - ducks[j].x);
        }
        int rowmindiff = 100000;
        if (grid[0].length() == ducks.size())
            rowmindiff = 0;
        for (int j = 0; j < grid[0].length() - ducks.size(); j++) {
            int rowdiff = 0;
            int fill[50] = {};
            for (int h = 0; h < ducks.size(); h++) {
                int mindiff = 10000;
                int mink = 0;
                for (int k = 0; k < ducks.size(); k++) {
                    if (fill[k])
                        continue;
                    if (mindiff > ABS(ducks[h].y-(j+k))) {
                        mindiff = ABS(ducks[h].y-(j+k));
                        mink = k;
                    }
                    cout << "d" <<mindiff <<  " " << ducks[h].y  << " " << j+k << endl;
                }
                fill[mink] = 1; 
                for (int k = 0; k < ducks.size(); k++) 
                    cout << fill[k] << " ";
                cout << endl;
                rowdiff += mindiff;
            }  
            rowmindiff = (rowmindiff < rowdiff ? rowmindiff : rowdiff);
            cout << rowmindiff << endl;
        }
        result = (result < rowmindiff + d ? result : rowmindiff + d);
    }
    for (int i = 0; i < grid[0].size(); i++) {
        int d = 0;
        for (int j = 0; j < ducks.size(); j++) {
            d += ABS(i - ducks[j].y);
        }
        int rowmindiff = 100000;
        if (grid.size() == ducks.size())
            rowmindiff = 0;
        for (int j = 0; j < grid.size() - ducks.size(); j++) {
            int rowdiff = 0;
            int fill[50] = {};
            for (int h = 0; h < ducks.size(); h++) {
                int mindiff = 10000;
                int mink = 0;
                for (int k = 0; k < ducks.size(); k++) {
                    if (fill[k])
                        continue;
                    if (mindiff > ABS(ducks[h].x-(j+k))) {
                        mindiff = ABS(ducks[h].x-(j+k));
                        mink = k;
                    }
                }
                fill[mink] = 1; 
                rowdiff += mindiff;
            }  
            rowmindiff = (rowmindiff < rowdiff ? rowmindiff : rowdiff);
        }
        result = (result < rowmindiff + d ? result : rowmindiff + d);
    }


    return result;
}

// BEGIN CUT HERE
int main() {
  DucksAlignment ___test;
  ___test.run_test(-1);
}
// END CUT HERE
