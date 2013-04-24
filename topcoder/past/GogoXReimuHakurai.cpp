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

class GogoXReimuHakurai {
public:
  int solve(vector <string> choices);

  

};

int way[51][51];
int memo[51];
int wayi;
int wayj;
int rec(int a)
{
    if (memo[a] != -1)
        return memo[a];

    int sum = 0;
    for (int j = a + 1; j < wayj; j++) {
        if (way[a][j]) {
            sum += rec(j);
        }
    }
    memo[a] = sum;
    return sum;
}
int GogoXReimuHakurai::solve(vector <string> choices) {
    wayi = choices.size();
    wayj = choices[0].length();
    for (int i = 0; i < 51; i++) 
        memo[i] = -1;
    memo[wayi-1] = 1;
    for (int i = 0; i < choices.size(); i++) 
        for (int j = 0; j < choices[0].length(); j++) 
            way[i][j] = (choices[i][j] == 'Y' ? 1 : 0);
    
    return rec(0); 
}



// Powered by FileEdit
// Powered by TZTester 1.01 [25-Feb-2003]
// Powered by CodeProcessor
