#ifndef __SENSOR_HPP_INCLUDED__
#define __SENSOR_HPP_INCLUDED__

#include "log.hpp"

class RandomSensor {
private:
    LogComponentRandom* log_component;
    int id;
public:
    RandomSensor(LogComponentRandom* log_component, int id);
    double FetchRandom(void);
};



#endif
