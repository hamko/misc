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

class FoxAndIntegers {
public:
  vector <int> get(int AminusB, int BminusC, int AplusB, int BplusC);

  

};

vector <int> FoxAndIntegers::get(int AminusB, int BminusC, int AplusB, int BplusC) {
    vector <int> result;

    int a = (AminusB+AplusB)/2;
    int b = (-AminusB+AplusB)/2;
    if (a + b != AplusB)
        return result;
    
    int b2 = (BminusC+BplusC)/2;
    int c = (-BminusC+BplusC)/2;
    if (b2 + c != BplusC)
        return result;

    if (b != b2)
        return result;
    
    result.push_back(a);
    result.push_back(b);
    result.push_back(c);

    return result;
}



// Powered by FileEdit
// Powered by TZTester 1.01 [25-Feb-2003]
// Powered by CodeProcessor
