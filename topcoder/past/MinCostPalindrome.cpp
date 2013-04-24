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

class MinCostPalindrome {
public:
  int getMinimum(string s, int oCost, int xCost) {
    int result;

	result = 0;
	for (int i = 0; i < s.length() / 2; i++) {
		char l = s[i];
		char r = s[s.length()-i-1];
		if (l == '?' && r == '?') {
			result += 2 * (oCost < xCost ? oCost : xCost);
		} else if (l == '?' && r == 'x') {
			result += xCost;
		} else if (l == '?' && r == 'o') {
			result += oCost;
		} else if (l == 'x' && r == '?') {
			result += xCost;
		} else if (l == 'o' && r == '?') {
			result += oCost;
		} else if (l == r) {
		} else {
			result = -1;
			break;
		}
	}

    return result;
  }
};



// Powered by FileEdit
// Powered by TZTester 1.01 [25-Feb-2003]
// Powered by CodeProcessor
