#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>
#include <limits.h>
#include <vector>
#include "poly.hpp"
#include "util.hpp"
#include <stdio.h>

using namespace std;

/* Poly */
void Poly::addMonoPoly(const MonoPoly* mono)
{
    MonoPoly tmp = *mono;
    m_func.push_back(tmp);
}

void Poly::initPolyAs(PolyInfrastructure* pi)
{
    m_func.clear();
    m_pi = pi;
}

Poly::Poly(PolyInfrastructure* pi)
{
    m_pi = pi;
}

Poly::Poly(PolyInfrastructure* pi, vector <MonoPoly> func)
{
    m_pi = pi;
    for (int i = 0; i < func.size(); i++) 
        m_func.push_back(func[i]);
}

const int Poly::getMonoPolyNum(void) const
{
    return m_func.size();
}

int Poly::getVarNum(void) const
{
    return m_pi->getVarNum();
}

const void Poly::Print(void) const
{
    if (!getMonoPolyNum()) {
        cout << "nil";
        return;
    }

    m_func[0].Print();
    for (int i = 1; i < this->getMonoPolyNum(); i++) {
        if (getMonoPolyWithIndex(i)->getCoeff() >= 0) 
            cout << " + ";
        m_func[i].Print();
    }
}

const MonoPoly* Poly::getMonoPolyWithIndex(int index) const
{
    return &m_func[index];
}

double MonoPoly::getCoeff(void) const
{
    return m_coeff;
}

void MonoPoly::setCoeff(double coeff)
{
    m_coeff = coeff;
}

MonoPoly* Poly::getMonoPolyWithIndex(int index) 
{
    return &m_func[index];
}

void Poly::setMonoPolyCoeffWithIndex(int index, double coeff) 
{
    m_func[index].setCoeff(coeff);
}

const vector<int> Poly::getPowerWithIndex(int index) const
{
    return m_func[index].getPower();
}


int Poly::searchSameDimMonoPoly(const MonoPoly* obj) const
{
    for (int i = 0; i < this->getMonoPolyNum(); i++) 
        if (obj->getPower() == getMonoPolyWithIndex(i)->getPower()) 
            return i;

    return -1;
}

// if this have a expression including obj, return the index of expression
int Poly::searchIncludeDimMonoPoly(const MonoPoly* obj) const
{
    for (int i = 0; i < getMonoPolyNum(); i++) {
        bool faf = true;
        for (int j = 0; j < m_pi->getVarNum(); j++) {
            if (!obj->getPower()[j])
                continue;
            if (obj->getPower()[j] > getMonoPolyWithIndex(i)->getPower()[j]) {
                faf = false;
                break;
            }
        }
        if (faf)
            return i;
    }

    return -1;
}

class EraseZeroPoly
{
public:
    bool operator()(MonoPoly mp) const {return DBL_EQ(mp.getCoeff(), 0.0); }
};
void Poly::cleanPoly(void)
{
    vector<MonoPoly>::iterator it = remove_if(m_func.begin(), m_func.end(), EraseZeroPoly());
    m_func.erase(it, m_func.end());
}

bool Poly::isZeroPoly(void) const
{
    return !m_func.size();
}

Poly Poly::operator+(const Poly& obj) const
{
    Poly tmp = Poly(this->m_pi);
    
    // This
    for (int i = 0; i < this->getMonoPolyNum(); i++) 
        tmp.addMonoPoly(this->getMonoPolyWithIndex(i));

    // obj
    for (int i = 0; i < obj.getMonoPolyNum(); i++) {
        int index;
        if ((index = tmp.searchSameDimMonoPoly(obj.getMonoPolyWithIndex(i))) == -1) {
            tmp.addMonoPoly(obj.getMonoPolyWithIndex(i));
        } else {
            tmp.setMonoPolyCoeffWithIndex(index, 
                    tmp.getMonoPolyWithIndex(index)->getCoeff() 
                  + obj.getMonoPolyWithIndex(i)    ->getCoeff());
        }
    }
    
    tmp.cleanPoly();
    return tmp;
}

Poly Poly::operator+(const MonoPoly& obj) const
{
    Poly tmp = Poly(this->m_pi);
    
    // This
    for (int i = 0; i < this->getMonoPolyNum(); i++) 
        tmp.addMonoPoly(this->getMonoPolyWithIndex(i));

    // obj
    int index;
    if ((index = tmp.searchSameDimMonoPoly(&obj)) == -1) {
        tmp.addMonoPoly(&obj);
    } else {
        tmp.setMonoPolyCoeffWithIndex(index, 
                tmp.getMonoPolyWithIndex(index)->getCoeff() 
              + obj.getCoeff());
    }
    
    tmp.cleanPoly();
    return tmp;
}

Poly Poly::operator-(const Poly& obj) const
{
    Poly tmp = Poly(this->m_pi);
    
    // This
    for (int i = 0; i < this->getMonoPolyNum(); i++) 
        tmp.addMonoPoly(this->getMonoPolyWithIndex(i));

    // obj
    for (int i = 0; i < obj.getMonoPolyNum(); i++) {
        int index;
        if ((index = tmp.searchSameDimMonoPoly(obj.getMonoPolyWithIndex(i))) == -1) {
            MonoPoly minus = -*(obj.getMonoPolyWithIndex(i));
            tmp.addMonoPoly(&minus);
        } else {
            tmp.setMonoPolyCoeffWithIndex(index, 
                    tmp.getMonoPolyWithIndex(index)->getCoeff() 
                  - obj.getMonoPolyWithIndex(i)    ->getCoeff());
        }
    }
    
    tmp.cleanPoly();
    return tmp;
}

Poly Poly::operator-(const MonoPoly& obj) const
{
    Poly tmp = Poly(this->m_pi);
    
    // This
    for (int i = 0; i < this->getMonoPolyNum(); i++) 
        tmp.addMonoPoly(this->getMonoPolyWithIndex(i));

    // obj
    int index;
    if ((index = tmp.searchSameDimMonoPoly(&obj)) == -1) {
        tmp.addMonoPoly(&obj);
    } else {
        tmp.setMonoPolyCoeffWithIndex(index, 
                tmp.getMonoPolyWithIndex(index)->getCoeff() 
              - obj.getCoeff());
    }
    
    tmp.cleanPoly();
    return tmp;
}

Poly Poly::operator*(const Poly& obj) const
{
    Poly tmp = Poly(this->m_pi);
    
    // This
    for (int i = 0; i < this->getMonoPolyNum(); i++) {
        for (int j = 0; j < obj.getMonoPolyNum(); j++) {
            MonoPoly toadd = *(obj.getMonoPolyWithIndex(j)) * *(this->getMonoPolyWithIndex(i));
            tmp = tmp + toadd;
        }
    }
    
    tmp.cleanPoly();
    return tmp;
}

Poly Poly::operator*(const MonoPoly& obj) const
{
    Poly tmp; tmp = *this * obj;

    tmp.cleanPoly();
    return tmp;
}
Poly Poly::operator*(double dbl) const
{
    Poly tmp = Poly(m_pi);
    
    // This
    for (int i = 0; i < getMonoPolyNum(); i++) {
        MonoPoly toadd = *(this->getMonoPolyWithIndex(i));
        toadd.setCoeff(toadd.getCoeff() * dbl);
        tmp = tmp + toadd;
    }
    
    tmp.cleanPoly();
    return tmp;
}

Poly Poly::operator/(double dbl) const
{
    Poly tmp = Poly(m_pi);
    
    // This
    for (int i = 0; i < getMonoPolyNum(); i++) {
        MonoPoly toadd = *(this->getMonoPolyWithIndex(i));
        toadd.setCoeff(toadd.getCoeff() / dbl);
        tmp = tmp + toadd;
    }
    
    tmp.cleanPoly();
    return tmp;
}

Poly Poly::operator()(const Poly& obj, string str) const
{
    // Copy
    Poly f; f = *this;
    
    // get one of the highest term.
    MonoPoly h0 = *(obj.getHighestTerm(str));
    double h0_coeff = h0.getCoeff();

    // calc.
    //      f - t + (t * h0_coeff / h0) * ((h0 - obj) / h0_coeff)
    // where 
    //      t = term including hterm
    while (1) {
        // search which includes hterm.
        int index;
        if ((index = f.searchIncludeDimMonoPoly(&h0)) == -1) { // if not break
            break;
        } else { // if any substitute
            MonoPoly t = *(f.getMonoPolyWithIndex(index));
            f = (f - t) + (t * h0_coeff / h0) * ((h0 - obj) / h0_coeff);
        }
    }

    f.cleanPoly();
    return f;
}

const MonoPoly* Poly::getHighestTerm(string str) const
{
    if (!getMonoPolyNum()) 
        return NULL;

    int maximum = INT_MIN;
    const MonoPoly* maxmp;
    int index = m_pi->getVarIndexNameOf(str); // index of PolyInfrastructure
    for (int i = 0; i < getMonoPolyNum(); i++) {
        const MonoPoly* mp = getMonoPolyWithIndex(i);
        int tmp = mp->getPowerWithIndex(index);
        if (maximum < tmp) {
            maximum = tmp;
            maxmp = mp;
        }
    }

    return maxmp;
}

PolyInfrastructure* Poly::getPolyInfrastructure(void) const
{
    return m_pi;
}

Poly& Poly::operator=(const MonoPoly& obj)
{
    initPolyAs(obj.getPolyInfrastructure());
    *this = *this + obj;
    return *this;
}

Poly Poly::operator-(void) const
{
    Poly tmp = Poly(this->m_pi);
    tmp = tmp - *this;

    return tmp;
}

