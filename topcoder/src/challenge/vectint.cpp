#include <string>
#include <vector>
#include <iostream>

using namespace std;

#define SIZEOFVECT 4

int output[SIZEOFVECT] = {
    1, 0, 2, 4,
};

int main(void)
{
    int i, j;
    for (i = 0; i < SIZEOFVECT; i++) {
        cout << output[i];
        if (i != SIZEOFVECT - 1)
            cout << ",";
    }
    cout << "\n";

    return 0;
}
