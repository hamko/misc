#ifndef __CONSOLE_HPP_INCLUDED__
#define __CONSOLE_HPP_INCLUDED__

#include "log.hpp"

class ConsoleComponent {
private:
    LogCollection* m_log_collection;
public:
    virtual void DisplayConsole(void) = 0;
    ConsoleComponent(LogCollection* log_collection);
    LogComponent* SearchLogComponent(int type, int id);
};

class ConsoleCollection {
private:
    std::vector <ConsoleComponent*> m_console_components;
public:
    ConsoleCollection();
    void AddConsoleComponent(ConsoleComponent* console_component);
    void DisplayConsoles(void);
};

class ConsoleDisplayRandomSensors : public ConsoleComponent {
private:
    double GetRandomSensorLog0(void);
    double GetRandomSensorLog1(void);
public:
    ConsoleDisplayRandomSensors(LogCollection* log_collection);
    void DisplayConsole(void);
};

#endif
