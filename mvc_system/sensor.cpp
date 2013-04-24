#include <iostream>
#include <stdlib.h>

#include "sensor.hpp"
RandomSensor::RandomSensor(LogComponentRandom* log_component, int id)
{
    this->log_component = log_component;
    this->id = id;
}

double RandomSensor::FetchRandom(void)
{
    double random = (double)rand() / RAND_MAX;
    if (log_component)
        log_component->AddRandomData(random);

    return random;
}
