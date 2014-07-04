#ifndef __MONOPOLY_HPP_INCLUDED__
#define __MONOPOLY_HPP_INCLUDED__

#include <string>
#include <vector>
#include "poly.hpp"
#include "polyinfrastructure.hpp"

using namespace std;

class PolyInfrastructure;
class Poly;

class MonoPoly {
private:
    PolyInfrastructure* m_pi;
    double m_coeff;
    vector <int> m_power;
public:
    MonoPoly(void){};
    MonoPoly(PolyInfrastructure* pi);
    MonoPoly(PolyInfrastructure* pi, string str, int pow, double coeff);
    MonoPoly(PolyInfrastructure* pi, vector <int> power, double coeff);
    PolyInfrastructure* getPolyInfrastructure(void) const;
    int getVarNum(void) const;
    void Print(void) const;
    vector<int> getPower(void) const;
    int getPowerWithIndex(int index) const;
    void setPowerWithIndex(int index, int power);
    double getCoeff(void) const;
    void setCoeff(double coeff);
    void cleanMonoPoly(void);
    Poly operator+(const MonoPoly& obj) const;
    Poly operator-(const MonoPoly& obj) const;
    MonoPoly operator-() const;
    MonoPoly operator*(const MonoPoly& obj) const;
    Poly operator*(const Poly& obj) const;
    MonoPoly operator*(double dbl) const;
    MonoPoly operator/(const MonoPoly& obj) const;
    MonoPoly operator/(double dbl) const;
    Poly operator-(const Poly& obj) const;
};

#endif
