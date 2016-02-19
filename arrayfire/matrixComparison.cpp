#include <arrayfire.h>
#include <iostream>
#include <Eigen/Core>

#define BITS 14

// Eigen
Eigen::MatrixXf A_Eigen;
int g_i;
static void calcEigen(void) {
    Eigen::MatrixXf dst = A_Eigen * A_Eigen;
    assert(dst.rows() == dst.cols());
}

// Eigen Parallel
static void calcEigenParallel(void) {
    Eigen::initParallel();
    Eigen::MatrixXf dst = A_Eigen * A_Eigen;
    assert(dst.rows() == dst.cols());
}

// arrayfire
af::array A;
static void calc(void)
{
    af::array dst = af::matmul(A, A);
    dst.eval();
}

void ev(void (*func)(void))
{
#if 0
    af::timer start1 = af::timer::start();
    func();
    double time = af::timer::stop(start1);
    double flops = 2.0 * powf(g_i, 3) / (time * 1.0e9);
    std::cout << g_i << " * " << g_i << " time: " << time << ", GFlops: " << flops << std::endl;

#else
    double time = af::timeit(func);
    double flops = 2.0 * powf(g_i, 3) / (time * 1.0e9);
    std::cout << g_i << " * " << g_i << " time: " << time << ", GFlops: " << flops << std::endl;
#endif
}

int main()
{
    // ArrayFire
    for(g_i = 1; g_i <= (1 << BITS); g_i <<= 1) {
        A = af::randu(g_i, g_i);
        ev(calc);
    }

    // Eigen
    for(g_i = 1; g_i <= (1 << BITS); g_i <<= 1) {
        A_Eigen = Eigen::MatrixXf::Random(g_i, g_i);
        ev(calcEigen);
    }

    /*
    // Eigen Parallel
    for(g_i = 1; g_i <= (1 << BITS); g_i <<= 1) {
        A_Eigen = Eigen::MatrixXf::Random(g_i, g_i);
        ev(calcEigenParallel);
    }
    */

    return 0;

}
