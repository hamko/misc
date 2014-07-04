#ifndef WU_METHOD_INCLUDED
#define WU_METHOD_INCLUDED

#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>
#include <limits.h>
#include <vector>
#include "poly.hpp"
#include "util.hpp"

class CompareWuVar {
public:
    bool operator()(pair <int, string> l, pair <int, string> r) {
        return l.first < r.first;
    }
};

class CompareWuPoly {
public:
    bool operator()(pair <int, Poly> l, pair <int, Poly> r) {
        return l.first < r.first;
    }
};

class WuMethod {
private:
    PolyInfrastructure* m_pi;
    vector <string> m_wuvar;
    vector <Poly> m_wupoly;
    Poly m_wuconc;
public:
    WuMethod(void){}
    WuMethod(PolyInfrastructure* pi, vector<string> wuvar, vector<Poly> wupoly, Poly wuconc);
    int getWuVarNum(void) const;
    int getWuPolyNum(void) const;
    void Print(void) const;
    bool Run(void);
    bool SingleRun(vector <pair <int, string> > varperm, vector <pair <int, Poly> > polyperm);
};

#endif
