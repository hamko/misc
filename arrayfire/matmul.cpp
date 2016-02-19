#include <arrayfire.h>
#include <iostream>

af::array A;
static void calc(void)
{
    af::array dst = af::matmul(A, A);
    dst.eval();
}

int main()
{
    for(int i = 1; i <= (1 << 14); i <<= 1) {
        A = af::randu(i, i);

        double time = af::timeit(calc);
        double flops = 2.0 * powf(i, 3) / (time * 1.0e9);

        std::cout << i << " * " << i << " time: " << time << ", GFlops: " << flops << std::endl;
    }

    return 0;

}
