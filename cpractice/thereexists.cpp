#include <iostream>
#define NUM 6

int main(int argc, char **argv)
{
    int x[NUM] = {1, 2, 3, 4, 5, 6};
    int y[NUM] = {1, 2, 3, 4, 5, 6};

    /**********************************************************************/
    // there exists i, j in [1, NUM], (x[i] + y[j]) % 3 == 0 ? tef = 0 : tef = 1;
    /**********************************************************************/
    int tef = 0; // there exists flag
    for (int i = 0; i < NUM; i++) {
        for (int j = 0; j < NUM; j++) {
            // condition
            if ((x[i] + y[i]) % 3 == 0)
                tef = 1;
            if (tef)
                break;
        }
        if (tef)
            break;
    }
    std::cout << tef << std::endl;

    return 0;
}
