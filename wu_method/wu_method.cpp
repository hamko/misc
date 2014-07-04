#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>
#include <limits.h>
#include <vector>
#include "poly.hpp"
#include "wu_method.hpp"
#include "util.hpp"

WuMethod::WuMethod(PolyInfrastructure* pi, vector<string> wuvar, vector<Poly> wupoly, Poly wuconc)
{
    m_pi = pi;
    m_wuvar = wuvar;
    m_wupoly = wupoly;
    m_wuconc = wuconc;
}

int WuMethod::getWuVarNum(void) const
{
    return m_wuvar.size();
}

int WuMethod::getWuPolyNum(void) const
{
    return m_wupoly.size();
}

void WuMethod::Print(void) const
{
    cout << "Vars: ";
    for (int i = 0; i < getWuVarNum(); i++) {
        cout << m_wuvar[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < getWuPolyNum(); i++) {
        cout << "Hyp: ";
        m_wupoly[i].Print();
        cout << " = 0" << endl;
    }
    cout << "Conc: ";
    m_wuconc.Print();
    cout << " = 0" << endl;
}

bool WuMethod::SingleRun(vector <pair <int, string> > varperm, vector <pair <int, Poly> > polyperm)
{
    vector <string> wuvar;
    vector <Poly> wupoly;
    for (int i = 0; i < varperm.size(); i++) 
        wuvar.push_back(varperm[i].second);
    for (int i = 0; i < varperm.size(); i++) 
        wupoly.push_back(polyperm[i].second);

    vector <Poly> tripoly;
    for (int i = 0; i < getWuVarNum(); i++) {
        tripoly.push_back(wupoly[i]);
        for (int j = 0; j < i; j++) {
            tripoly[i] = tripoly[i](tripoly[j], wuvar[j]);
//            tripoly[i].Print(); cout << "(mod "; tripoly[j].Print(); cout << ") " << wuvar[j] << " #tri" << j << endl;
        }

        // if there is variable not needed, return false
        for (int j = 0; j < i; j++) {
            MonoPoly single = MonoPoly(m_pi, wuvar[j], 1, 1.0);
            if (tripoly[i].searchIncludeDimMonoPoly(&single) != -1) {
                cout << "failed triangulation." << endl;
                return false;
            }
        }
    }
    
    // if there is doubled tripoly, return false
    for (int i = 0; i < tripoly.size(); i++) {
        if (tripoly[i].isZeroPoly()) {
            cout << "failed for 0 poly in tripoly." << endl;
            return false;
        }
    }

    for (int i = 0; i < tripoly.size(); i++) {
        cout << "Tri: ";
        tripoly[i].Print();
        cout << " = 0" << endl;
    }

    Poly conc = m_wuconc;
    for (int i = 0; i < getWuVarNum(); i++) {
        conc = conc(tripoly[i], wuvar[i]);
        // if there is variable not needed, return false
        for (int j = 0; j < i; j++) {
            MonoPoly single = MonoPoly(m_pi, wuvar[j], 1, 1.0);
            if (conc.searchIncludeDimMonoPoly(&single) != -1) {
                conc.Print(); cout << endl;
                cout << "failed for unexpected reminder." << endl;
                return false;
            }
        }
    }

    if (conc.isZeroPoly()) {
        return true;
    } else {
        cout << "failed for unexpected reminder at the end." << endl;
        conc.Print(); cout << endl;
        return false;
    }
}

bool WuMethod::Run(void)
{
    vector <pair <int, string> > varperm;
    vector <pair <int, Poly> > polyperm;
    for (int i = 0; i < getWuVarNum(); i++) 
        varperm.push_back(make_pair(i, m_wuvar[i]));
    for (int i = 0; i < getWuPolyNum(); i++) 
        polyperm.push_back(make_pair(i, m_wupoly[i]));

    do {
        do {
            if (SingleRun(varperm, polyperm))
                return true;
        } while (next_permutation(varperm.begin(), varperm.end(), CompareWuVar()));
    } while (next_permutation(polyperm.begin(), polyperm.end(), CompareWuPoly()));
    
    return false;
}

