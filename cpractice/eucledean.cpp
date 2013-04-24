#include <stdio.h>

int gcd(int a, int b)
{
    if (!b)
        return a;
    return gcd(b, a % b);
}

int main(int argc, char **argv)
{
    printf("3 5 -> %d\n", gcd(3, 5));
    printf("8 6 -> %d\n", gcd(8, 6));

    return 0;
}
