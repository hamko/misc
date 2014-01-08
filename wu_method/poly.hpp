#ifndef __POLY_HPP_INCLUDED__
#define __POLY_HPP_INCLUDED__

#include <string>
#include <vector>
#include "polyinfrastructure.hpp"
#include "monopoly.hpp"
#include "operand.hpp"
using namespace std;

class Poly : public Operand {
private: 
    vector <MonoPoly> m_func;
    PolyInfrastructure* m_pi;
    void addMonoPoly(const MonoPoly* mono);
public:
    void initPolyAs(PolyInfrastructure* pi);
    const int getMonoPolyNum(void) const;
    MonoPoly* getMonoPolyWithIndex(int index);
    const MonoPoly* getMonoPolyWithIndex(int index) const;
    void setMonoPolyCoeffWithIndex(int index, double coeff);
    int searchSameDimMonoPoly(const MonoPoly* obj) const;
    int searchIncludeDimMonoPoly(const MonoPoly* obj) const;
    const MonoPoly* getHighestTerm(string str) const;
    const vector<int> getPowerWithIndex(int index) const;
    bool isZeroPoly(void) const;
    int getVarNum(void) const;
    const void Print(void) const;
    void cleanPoly(void);
    Poly(void){};
    Poly(PolyInfrastructure* pi);
    PolyInfrastructure* getPolyInfrastructure(void) const;
    Poly(PolyInfrastructure* pi, vector <MonoPoly> func);
    Poly operator()(const Poly& obj, string str) const; // mod
    Poly operator+(const Poly& obj) const;
    Poly operator+(const MonoPoly& obj) const;
    Poly operator-(const Poly& obj) const;
    Poly operator-(const MonoPoly& obj) const;
    Poly operator-(void) const;
    Poly operator*(double dbl) const;
    Poly operator*(const Poly& obj) const;
    Poly operator*(const MonoPoly& obj) const;
    Poly operator/(double dbl) const;
    Poly& operator=(const MonoPoly& obj);
};

#endif
