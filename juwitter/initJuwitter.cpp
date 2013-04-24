#include <iostream>
#include "twitterGetTimeline.hpp"
#include "julius-simple.hpp"

using namespace std;

void start_juwitter(void)
{
    cout << "called start_juwitter" << endl;
    cout << "create thread juwitter_post_main" << endl;
    pthread_t postp;
    pthread_create(&postp, NULL, juwitter_post_main, NULL);
//    pthread_join(postp, NULL);

    cout << "create thread juwitter_get_main" << endl;
    pthread_t getp;
    pthread_create(&getp, NULL, juwitter_get_main, NULL);
//    pthread_join(getp, NULL);

    while (1) {
        sleep(INT_MAX);
    }
}

int main(void)
{
    start_juwitter();
    return 0;
}
