#include <string>
#include <vector>
#include <iostream>

using namespace std;

#define SIZEOFVECT 3
#define SIZEOFSTR 3

string output[SIZEOFVECT] = {
    "AAA", 
    "ABA", 
    "BBB", 
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
