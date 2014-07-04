#ifndef __POLYINFRASTRUCTURE_HPP_INCLUDED__
#define __POLYINFRASTRUCTURE_HPP_INCLUDED__

#include <string>
#include <vector>
#include "poly.hpp"
#include "monopoly.hpp"

using namespace std;

class PolyInfrastructure {
private:
    vector <string> m_var;
public:
    void addVar(string str);
    int getVarNum(void);
    string getVarNameOf(int index);
    int getVarIndexNameOf(string str);
    PolyInfrastructure(void);
    PolyInfrastructure(vector <string> var);
};


#endif
