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

class GogoXCake {
public:
  string solve(vector <string> cake, vector <string> cutter);

  

};

int flag = 0;
int ca[51][51];
int cu[51][51];
int csizei;
int csizej;
int nsizei;
int nsizej;

int _max(int a, int b)
{
    return (a > b ? a : b);
}


/*
class array50x50 {
public:
    array50x50(const array50x50& another) {
        for (int i = 0
    }
    int value[50][50];
    int& at(i, j) { return value[i][j]; }
};
*/

void rec(int input[51][51])
{
/*
    for (int i = 0; i < cake.size(); i++) {
        for (int j = 0; j < cake[0].length(); j++) {
            ca[i][j] = cake[i][j] == 'X';
            cout << ca[i][j];
        }
        cout << endl;
    }
    cout << endl;
*/
 
    // end
    int endflag = 1;
    for (int a = 0; a < csizei; a++) {
        for (int b = 0; b < csizej; b++) {
            if (!input[a][b]) {
                endflag = 0;
                break;
            }
        }
        if (!endflag)
            break;
    }
    flag = endflag;
    if (flag)
        return;

    for (int a = 0; a < csizei - nsizei + 1; a++) {
        for (int b = 0; b < csizej - nsizej + 1; b++) {
            int cdflag = 1;
            for (int c = 0; c < nsizei; c++) {
                for (int d = 0; d < nsizej; d++) {
                    if (input[a+c][b+d] && cu[c][d]) {// out condition
                        cdflag = 0;
                        break;
                    }
                }
                if (!cdflag)
                    break;
            }
            if (cdflag) { // ok
                int (*_input)[51];
                _input = (int (*)[51])malloc(sizeof(int) * (51 * 51));
                for (int i = 0; i < csizei; i++) {
                    for (int j = 0; j < csizej; j++) {
                        _input[i][j] = input[i][j];
                    }
                }
                for (int c = 0; c < nsizei; c++) {
                    for (int d = 0; d < nsizej; d++) {
                        _input[a+c][b+d] = _max(input[a+c][b+d], cu[c][d]);
                    }
                }
                rec(_input);
                free(_input);
/*
                int _input[51][51];
                for (int i = 0; i < csizei; i++) {
                    for (int j = 0; j < csizej; j++) {
                        _input[i][j] = input[i][j];
                    }
                }
                for (int c = 0; c < nsizei; c++) {
                    for (int d = 0; d < nsizej; d++) {
                        _input[a+c][b+d] = _max(input[a+c][b+d], cu[c][d]);
                    }
                }
                rec(_input);
*/
            }
        }
    }

}

string GogoXCake::solve(vector <string> cake, vector <string> cutter) {
    csizei = cake.size();
    csizej = cake[0].length();
    nsizei = cutter.size();
    nsizej = cutter[0].length();
    for (int i = 0; i < (int)cake.size(); i++) {
        for (int j = 0; j < (int)(cake[0].length()); j++) {
            ca[i][j] = cake[i][j] == 'X';
            cout << ca[i][j];
        }
        cout << endl;
    }
    
    for (int i = 0; i < (int)cutter.size(); i++) {
        for (int j = 0; j < (int)(cutter[0].length()); j++) {
            cu[i][j] = cutter[i][j] == '.';
            cout << cu[i][j];
        }
        cout << endl;
    }

    rec(ca);
    return (flag ? "YES" : "NO");
}

int main(void) 
{
    vector <string> cake;
    vector <string> cutter;
/*
    for (int i = 0; i < 50; i++)
        cake.push_back("..................................................");
    cutter.push_back(".");
*/
    for (int i = 0; i < 5; i++)
        cake.push_back("....");
    cutter.push_back("..");

    GogoXCake test;
    cout << test.solve(cake, cutter) << endl;
    return 0;
}


