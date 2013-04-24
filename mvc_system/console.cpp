#include <iostream>
#include <stdlib.h>
#include <vector>

#include "console.hpp"
#include "log.hpp"
#include "option.hpp"

using namespace std;

// Console Collection
ConsoleCollection::ConsoleCollection()
{
}
void ConsoleCollection::AddConsoleComponent(ConsoleComponent* console_component)
{
    m_console_components.push_back(console_component);
}
void ConsoleCollection::DisplayConsoles(void)
{
    for (int i = 0; i < m_console_components.size(); i++) 
        m_console_components[i]->DisplayConsole();
}

// Console Component
ConsoleComponent::ConsoleComponent(LogCollection* log_collection)
{
    this->m_log_collection = log_collection;
}

LogComponent* ConsoleComponent::SearchLogComponent(int type, int id)
{
    return m_log_collection->SearchLogComponent(type, id);
}

// Console Display Random Sensors
ConsoleDisplayRandomSensors::ConsoleDisplayRandomSensors(LogCollection* log_collection)
    : ConsoleComponent(log_collection)
{
}

double ConsoleDisplayRandomSensors::GetRandomSensorLog0(void)
{
    return ((LogComponentRandom*)SearchLogComponent(LC_RANDOM, 0))->GetLogComponentStoreLast();
}

double ConsoleDisplayRandomSensors::GetRandomSensorLog1(void)
{
    return ((LogComponentRandom*)SearchLogComponent(LC_RANDOM, 1))->GetLogComponentStoreLast();
}

void ConsoleDisplayRandomSensors::DisplayConsole(void) // これが本当の描画関数
{
    cout    << GetRandomSensorLog0() + GetRandomSensorLog1() << " #plus" << endl;
}
