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

class KingSort {
public:
  vector <string> getSortedList(vector <string> kings);

  

};

typedef struct _king {
    char name[1024];
    int num;
} king_t;
king_t k[51];

int strtonum(char *numstr)
{
    return 0;
}

vector <string> KingSort::getSortedList(vector <string> kings) {
    vector <string> result;

    int n = kings.size();
    for (int i = 0; i < n; i++) {
        char name[1024], char numstr[1024];
        sscanf(kings[i].c_str(), "%s %s", name, numstr);
        strcpy(k[i].name, name);
        k[i].num = strtonum(numstr);
        printf("%s %s\n", k[i].name, k[i].num);
    }

    return result;
}



// Powered by FileEdit
// Powered by TZTester 1.01 [25-Feb-2003]
// Powered by CodeProcessor
