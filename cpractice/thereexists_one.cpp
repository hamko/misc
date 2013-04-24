#include <iostream>
#define NUM 6

int main(int argc, char **argv)
{
    int x[NUM] = {1, 2, 3, 4, 5, 6};
    int y[NUM] = {1, 2, 3, 4, 5, 6};

    /**********************************************************************/
    // there exists i in [1, NUM], (x[i] + y[i]) % 7 == 0 ? tef = 0 : tef = 1;
    /**********************************************************************/
    int i;
    for (i = 0; i < NUM; i++) {
        // condition
        if ((x[i] + y[i]) % 7 == 0)
            break;
    }
    int tef = (i != NUM);
    std::cout << tef << std::endl;

    return 0;
}
