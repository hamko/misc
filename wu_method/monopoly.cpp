#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>
#include <limits.h>
#include <vector>
#include "poly.hpp"
#include "util.hpp"

using namespace std;

/* Mono Poly */
MonoPoly::MonoPoly(PolyInfrastructure* pi)
{
    m_pi = pi;
    for (int i = 0; i < m_pi->getVarNum(); i++) {
        m_power.push_back(0);
    }
}

MonoPoly::MonoPoly(PolyInfrastructure* pi, string str, int pow, double coeff)
{
    m_pi = pi;
    for (int i = 0; i < m_pi->getVarNum(); i++) 
        m_power.push_back(0);

    m_power[pi->getVarIndexNameOf(str)] = pow;
    m_coeff = coeff;
}

int MonoPoly::getVarNum(void) const
{
    return m_pi->getVarNum();
}

MonoPoly::MonoPoly(PolyInfrastructure* pi, vector <int> power, double coeff)
{
    m_pi = pi;
    assert((int)power.size() == m_pi->getVarNum());

    for (int i = 0; i < m_pi->getVarNum(); i++) {
        m_power.push_back(power[i]);
    }
    m_coeff = coeff;
}

vector<int> MonoPoly::getPower(void) const
{
    return m_power;
}

int MonoPoly::getPowerWithIndex(int index) const
{
    return m_power[index];
}
void MonoPoly::setPowerWithIndex(int index, int power)
{
    m_power[index] = power;
}

PolyInfrastructure* MonoPoly::getPolyInfrastructure(void) const
{
    return m_pi;
}

void MonoPoly::Print(void) const
{
    // Print Coefficient
    if (DBL_EQ(m_coeff, 0.0)) { // for zero
        cout << 0.0;
        cerr << "#WARNING: not correctly cleaned poly!" << endl;
        return;
    } else if (DBL_EQ(m_coeff, -1.0)) { // for one coefficient
        cout << "-";
    } else if (DBL_EQ(m_coeff, 1.0)) {
    } else {
        cout << m_coeff << " ";
    }

    // for constant term
    bool one_flag = true; 
    for (int i = 0; i < m_pi->getVarNum(); i++) {
        if (m_power[i]) {
            one_flag = false;
            break;
        }
    }
    if (one_flag) {
        cout << m_coeff;
        return;
    }

    for (int i = 0; i < m_pi->getVarNum(); i++) {
        if (m_power[i]) {
            cout << m_pi->getVarNameOf(i);
            if (m_power[i] > 1)
                cout << "^" << m_power[i];
            one_flag = 0;
            if (i != m_pi->getVarNum() - 1)
                cout << " ";
        }
    }
}

void MonoPoly::cleanMonoPoly(void)
{
    for (int i = 0; i < m_pi->getVarNum(); i++) 
        m_power[i] = 0;
}
MonoPoly MonoPoly::operator*(const MonoPoly& obj) const
{
    MonoPoly tmp = MonoPoly(m_pi);

    bool faf = true;
    for (int i = 0; i < m_pi->getVarNum(); i++) {
        int order = getPowerWithIndex(i) + obj.getPowerWithIndex(i);
        if (order) 
            faf = false;
            
        tmp.setPowerWithIndex(i, order);
    }
    if (faf) 
        tmp.cleanMonoPoly();

    tmp.setCoeff(this->getCoeff() * obj.getCoeff());

    return tmp;
}

MonoPoly MonoPoly::operator/(const MonoPoly& obj) const
{
    MonoPoly tmp = MonoPoly(m_pi);

    bool faf = true;
    for (int i = 0; i < m_pi->getVarNum(); i++) {
        int order = getPowerWithIndex(i) - obj.getPowerWithIndex(i);
        if (order) 
            faf = false;
            
        tmp.setPowerWithIndex(i, order);
    }
    if (faf) 
        tmp.cleanMonoPoly();

    tmp.setCoeff(getCoeff() / obj.getCoeff());

    return tmp;
}

MonoPoly MonoPoly::operator*(double dbl) const
{
    MonoPoly tmp = *this;
    tmp.setCoeff(tmp.getCoeff() * dbl);

    return tmp;
}

Poly MonoPoly::operator*(const Poly& obj) const
{
    Poly tmp; tmp = *this;
    return tmp * obj;
}

Poly MonoPoly::operator+(const MonoPoly& obj) const
{
    Poly tmp = Poly(m_pi);
    tmp = *this;
    return tmp + obj;
}

Poly MonoPoly::operator-(const MonoPoly& obj) const
{
    Poly tmp = Poly(m_pi);
    tmp = *this;
    return tmp - obj;
}

MonoPoly MonoPoly::operator-() const
{
    MonoPoly tmp = *this;
    tmp.m_coeff *= -1;
    return tmp;
}


MonoPoly MonoPoly::operator/(double dbl) const
{
    MonoPoly tmp = *this;
    tmp.setCoeff(tmp.getCoeff() / dbl);

    return tmp;
}

Poly MonoPoly::operator-(const Poly& obj) const
{
    return -obj + *this;
}
