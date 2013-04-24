#include <iostream>
#define NUM 6

int main(int argc, char **argv)
{
    int x[NUM] = {1, 2, 3, 4, 5, 6};
    int y[NUM] = {1, 2, 3, 4, 5, 6};

    /**********************************************************************/
    // for all i, j in [1, NUM], (x[i] + y[j]) % 2 == 0 ? faf = 1 : faf = 0;
    /**********************************************************************/
    int faf = 1; // for all flag
    for (int i = 0; i < NUM; i++) {
        for (int j = 0; j < NUM; j++) {
            // cont-condition
            if (!((x[i] + y[i]) % 2 == 0)) 
                faf = 0;
            if (!faf)
                break;
        }
        if (!faf)
            break;
    }
    std::cout << faf << std::endl;

    return 0;
}
