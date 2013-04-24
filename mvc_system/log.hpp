#ifndef __LOG_HPP_INCLUDED__
#define __LOG_HPP_INCLUDED__

#include <vector>

class LogComponent {
protected:
    int type;
    int id;
public:
    LogComponent(int type, int id);
    int GetType(void);
    int GetId(void);
};

class LogComponentRandom : public LogComponent {
private:
    std::vector<double> store;
public:
    LogComponentRandom(int type, int id);
    void AddRandomData(double input);
    double GetLogComponentStoreLast(void);
};

class LogCollection {
private:
    std::vector <LogComponent*> m_logcomponents;
public:
    LogCollection(void);
    void AddLogComponent(LogComponent* logcomponent);
    LogComponent* SearchLogComponent(int type, int id);
};
#endif
