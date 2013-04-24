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

class PairingPawns {
public:
  int savedPawnCount(vector <int> start);

  

};

int PairingPawns::savedPawnCount(vector <int> start) {
    for (int i = start.size() - 1; i > 0; i--) 
        start[i-1] += start[i]/2;
    
    return start[0];
}



// Powered by FileEdit
// Powered by TZTester 1.01 [25-Feb-2003]
// Powered by CodeProcessor
