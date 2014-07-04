#include <iostream>
#include "poly.hpp"
#include "wu_method.hpp"
#include "util.hpp"
#include <assert.h>

Poly calcMP(PolyInfrastructure* pi, const string var) 
{
    Poly tmp; tmp = MonoPoly(pi, var, 1, 1.0);
    return tmp;
}
Poly calcSQ(Poly input)
{
    return input * input;
}

int main(void)
{
    PolyInfrastructure pi;
    Poly f1, f2, f3, f4;
    vector<string> wuvar;
    vector<Poly> wupoly;
    Poly wuconc;
    WuMethod wm = WuMethod();
    vector <string> var_names;

    cout << "/*** Q1 ***/" << endl;
    var_names.clear();
    var_names.push_back("a");
    var_names.push_back("b");
    var_names.push_back("c");
    var_names.push_back("d");
    var_names.push_back("e");

    pi = PolyInfrastructure(var_names);
    f1 = Poly(&pi);
    f1 = f1 + MonoPoly(&pi, "e", 1, 1.0) + MonoPoly(&pi, "a", 1, -2.0);
    f2 = Poly(&pi);
    f2 = f2 + MonoPoly(&pi, "a", 1, 2.0) + MonoPoly(&pi, "b", 1, -1.0) - MonoPoly(&pi, "d", 1, -1.0);
    wuconc = Poly(&pi);
    wuconc = wuconc + calcSQ(calcMP(&pi, "a") - calcMP(&pi, "b")) - calcSQ(calcMP(&pi, "a") - calcMP(&pi, "d"));

    wuvar.clear();
    wuvar.push_back("a");
    wuvar.push_back("b");

    wupoly.clear();
    wupoly.push_back(f1);
    wupoly.push_back(f2);

    wm = WuMethod(&pi, wuvar, wupoly, wuconc);
    wm.Print();
    cout << (wm.Run() ? "TRUE" : "FALSE") << endl;

    cout << "/*** Q2 ***/" << endl;
    var_names.clear();
    var_names.push_back("a");
    var_names.push_back("b");
    var_names.push_back("c");
    var_names.push_back("d");
    var_names.push_back("e");

    pi = PolyInfrastructure(var_names);
    f1 = Poly(&pi);
    f1 = f1 + MonoPoly(&pi, "e", 1, 1.0) + MonoPoly(&pi, "a", 1, -2.0);
    f2 = Poly(&pi);
    f2 = f2 + MonoPoly(&pi, "d", 1, 1.0) + MonoPoly(&pi, "b", 1, -1.0);
    wuconc = Poly(&pi);
    wuconc = wuconc + calcSQ(calcMP(&pi, "a") - calcMP(&pi, "b")) - calcSQ(calcMP(&pi, "a") - calcMP(&pi, "d"));

    wuvar.clear();
    wuvar.push_back("a");
    wuvar.push_back("b");

    wupoly.clear();
    wupoly.push_back(f1);
    wupoly.push_back(f2);

    wm = WuMethod(&pi, wuvar, wupoly, wuconc);
    wm.Print();
    cout << (wm.Run() ? "TRUE" : "FALSE") << endl;

    return 0;
}
