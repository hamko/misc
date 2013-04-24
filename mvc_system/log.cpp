#include <iostream>
#include <vector>

#include "option.hpp"
#include "log.hpp"

using namespace std;

// Log Component
LogComponent::LogComponent(int type, int id)
{
    this->type = type;
    this->id = id;
}

int LogComponent::GetType(void)
{
    return type;
}

int LogComponent::GetId(void)
{
    return id;
}

// Log Collection
LogCollection::LogCollection(void) 
{
    return;
}

void LogCollection::AddLogComponent(LogComponent* logcomponent) 
{
    m_logcomponents.push_back(logcomponent);
}

LogComponent* LogCollection::SearchLogComponent(int type, int id)
{
    int i;
    for (i = 0; i < m_logcomponents.size(); i++) 
        if (m_logcomponents[i]->GetType() == type && m_logcomponents[i]->GetId() == id)
            break;
    
    return (i == m_logcomponents.size() ? NULL : m_logcomponents[i]);
}

// Log Component Random
LogComponentRandom::LogComponentRandom(int type, int id) : LogComponent(type, id)
{
}

void LogComponentRandom::AddRandomData(double input)
{
    store.push_back(input);
}

double LogComponentRandom::GetLogComponentStoreLast(void)
{
    if (!store.size()) {
        cerr << "#warning: not stored in random dat" << endl;
    }
    return store[store.size() - 1];
}
