#include <iostream>
#include "log.hpp"
#include "sensor.hpp"
#include "option.hpp"
#include "console.hpp"

using namespace std;

int main(int argc, char** argv)
{
    // LogComponentをまとめるためのオブジェクト
    LogCollection* log_collection = new LogCollection();

    // Random Sensor0 (r0)の設定
    LogComponentRandom* lcr0 = new LogComponentRandom(LC_RANDOM, 0);
    RandomSensor* r0 = new RandomSensor(lcr0, 0);
    log_collection->AddLogComponent(lcr0);

    // Random Sensor1 (r1)の設定
    LogComponentRandom* lcr1 = new LogComponentRandom(LC_RANDOM, 1);
    RandomSensor* r1 = new RandomSensor(lcr1, 1);
    log_collection->AddLogComponent(lcr1);

    // ConsoleComponentをまとめるためのオブジェクト
    ConsoleCollection* console_collection = new ConsoleCollection();

    // ConsoleDisplayRandomSensorsの設定
    ConsoleDisplayRandomSensors* console_drs = new ConsoleDisplayRandomSensors(log_collection);
    console_collection->AddConsoleComponent(console_drs);

    for (int i = 0; i < 10; i++) {
        cout << r0->FetchRandom() << " " << r1->FetchRandom() << " #fetched" << endl;
        console_collection->DisplayConsoles();
    }

    return 0;
}
