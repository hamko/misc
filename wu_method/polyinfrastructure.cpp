#include <iostream>
#include <algorithm>
#include <assert.h>
#include <string>
#include <limits.h>
#include <vector>
#include "polyinfrastructure.hpp"
#include "util.hpp"

/* PolyInfrastructure */
void PolyInfrastructure::addVar(string str)
{
    m_var.push_back(str);
}

int PolyInfrastructure::getVarNum(void)
{
    return m_var.size();
}

int PolyInfrastructure::getVarIndexNameOf(string str)
{
    for (int i = 0; i < m_var.size(); i++) 
        if (m_var[i] == str)
            return i;

    return -1;
}

string PolyInfrastructure::getVarNameOf(int index)
{
    assert(index >= 0);
    assert(index < m_var.size());

    return m_var[index];
}

PolyInfrastructure::PolyInfrastructure(void)
{
}

PolyInfrastructure::PolyInfrastructure(vector <string> var)
{
    for (int i = 0; i < var.size(); i++) {
        m_var.push_back(var[i]);
    }
}
