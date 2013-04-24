#include <iostream>
#define NUM 6

int main(int argc, char **argv)
{
    int x[NUM] = {1, 2, 3, 4, 5, 6};
    int y[NUM] = {1, 2, 3, 4, 5, 6};

    /**********************************************************************/
    // for all i in [1, NUM], (x[i] + y[i]) % 2 == 0 ? faf = 1 : faf = 0;
    /**********************************************************************/
    int i;
    for (i = 0; i < NUM; i++) {
        // cont-condition
        if (!((x[i] + y[i]) % 2 == 0)) 
            break;
    }
    int faf = (i == NUM);
    std::cout << faf << std::endl;

    return 0;
}
